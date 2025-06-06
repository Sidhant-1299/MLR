// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MAIN
#define BOOST_TEST_IGNORE_SIGCHLD
#include <boost/test/included/unit_test.hpp>

#include <boost/process/v1/child.hpp>
#include <boost/process/v1/environment.hpp>
#include <boost/process/v1/filesystem.hpp>
#include <boost/process/v1/io.hpp>
#include <boost/process/v1/pipe.hpp>
#include <boost/process/v1/posix.hpp>
#include <boost/process/v1/spawn.hpp>
#include <boost/process/v1/system.hpp>

#if !defined(BOOST_PROCESS_USE_STD_FS)
#include <boost/filesystem/directory.hpp>
#endif


#include <system_error>


#include <string>
#include <sys/wait.h>
#include <errno.h>

namespace fs = boost::process::v1::filesystem;
namespace bp = boost::process::v1;

BOOST_AUTO_TEST_CASE(bind_fd, *boost::unit_test::timeout(2))
{
    using boost::unit_test::framework::master_test_suite;

    bp::pipe p;

    std::error_code ec;
    bp::child c(
        master_test_suite().argv[1],
        "test", "--posix-echo-one", "3", "hello",
        bp::posix::fd.bind(3, p.native_sink()),
        ec
    );
    BOOST_CHECK(!ec);


    bp::ipstream is(std::move(p));

    std::string s;
    is >> s;
    BOOST_CHECK_EQUAL(s, "hello");
}

BOOST_AUTO_TEST_CASE(bind_fds, *boost::unit_test::timeout(2))
{
    using boost::unit_test::framework::master_test_suite;

    bp::pipe p1;
    bp::pipe p2;

    std::error_code ec;
    bp::child c(
        master_test_suite().argv[1],
        "test","--posix-echo-two","3","hello","99","bye",
        bp::posix::fd.bind(3,  p1.native_sink()),
        bp::posix::fd.bind(99, p2.native_sink()),
        ec
    );
    BOOST_CHECK(!ec);

    bp::ipstream is1(std::move(p1));
    bp::ipstream is2(std::move(p2));

    std::string s1;
    is1 >> s1;
    BOOST_CHECK_EQUAL(s1, "hello");

    std::string s2;
    is2 >> s2;
    BOOST_CHECK_EQUAL(s2, "bye");
}

BOOST_AUTO_TEST_CASE(execve_set_on_error, *boost::unit_test::timeout(2))
{
    std::error_code ec;
    bp::spawn(
        "doesnt-exist",
        ec
    );
    BOOST_CHECK(ec);
    BOOST_CHECK_EQUAL(ec.value(), ENOENT);
}

BOOST_AUTO_TEST_CASE(execve_throw_on_error, *boost::unit_test::timeout(2))
{
    try
    {
        bp::spawn("doesnt-exist");
        BOOST_CHECK(false);
    }
    catch (bp::process_error &e)
    {
        BOOST_CHECK(e.code());
        BOOST_CHECK_EQUAL(e.code().value(), ENOENT);
    }
}

BOOST_AUTO_TEST_CASE(leak_test, *boost::unit_test::timeout(5))
{
    using boost::unit_test::framework::master_test_suite;

    std::error_code ec;

    const auto pid = boost::this_process::get_id();
    const auto  fd_path = fs::path("/proc") / std::to_string(pid) / "fd";

    auto get_fds = [&]{
        std::vector<int> fds;
        for (auto && fd : fs::directory_iterator(fd_path))
            fds.push_back(std::stoi(fd.path().filename().string()));
        return fds;
    };

    std::vector<int> fd_list = get_fds();
    if (fd_list.empty()) //then there's no /proc in the current linux distribution.
        return;


    BOOST_CHECK(std::find(fd_list.begin(), fd_list.end(), STDOUT_FILENO) != fd_list.end());
    BOOST_CHECK(std::find(fd_list.begin(), fd_list.end(),  STDIN_FILENO) != fd_list.end());
    BOOST_CHECK(std::find(fd_list.begin(), fd_list.end(), STDERR_FILENO) != fd_list.end());

    bp::pipe p; //should add two descriptors.

    auto fd_list_new = get_fds();

    BOOST_CHECK_EQUAL(fd_list_new.size(), fd_list.size() + 2);
    fd_list.push_back(p.native_source());
    fd_list.push_back(p.native_sink());


    BOOST_CHECK_EQUAL(
            bp::system(
                    master_test_suite().argv[1],
                    "test", "--exit-code", "123",  ec), 123);

    fd_list_new = get_fds();
    BOOST_CHECK_EQUAL(fd_list.size(), fd_list_new.size());


    const int native_source = p.native_source();
    BOOST_CHECK_EQUAL(
            bp::system(
                    master_test_suite().argv[1],
                    bp::std_in < p,
                    "test", "--exit-code", "123",  ec), 123);

    BOOST_CHECK(!ec);

   ////now, p.source should be closed, so we remove it from fd_list

   const auto itr = std::find(fd_list.begin(), fd_list.end(), native_source);
   if (itr != fd_list.end())
       fd_list.erase(itr);

    fd_list_new = get_fds();

    BOOST_CHECK_EQUAL(fd_list.size(), fd_list_new.size());

}

BOOST_AUTO_TEST_CASE(pipe_fd_is_not_leaked, *boost::unit_test::timeout(5))
{
    const std::string partner = boost::unit_test::framework::master_test_suite().argv[1];

    #define test(parent_closes_source, fd_to_check, expected_exitcode, call_args...) \
        do { \
            boost::asio::io_context ioctx; \
            bp::async_pipe pipe(ioctx); \
            BOOST_CHECK_NE(pipe.native_source(), -1); \
            BOOST_CHECK_NE(pipe.native_sink(), -1); \
            BOOST_CHECK_EQUAL(bp::system(partner, "--has-handle", std::to_string(fd_to_check), call_args), expected_exitcode); \
            if (parent_closes_source) { \
                BOOST_CHECK_EQUAL(pipe.native_source(), -1); \
                BOOST_CHECK_NE(pipe.native_sink(), -1); \
            } else { \
                BOOST_CHECK_NE(pipe.native_source(), -1); \
                BOOST_CHECK_EQUAL(pipe.native_sink(), -1); \
            } \
        } while (0)

    // Both parent and child processes must close the end of the pipe which they don't use,
    // and pipe_in/pipe_out must close the original source/sink fd after redirecting in the child process,
    // so that they don't keep the pipe open unnecessarily.
    test(true, STDIN_FILENO        , EXIT_SUCCESS, bp::std_in < pipe);
    test(true, pipe.native_source(), EXIT_FAILURE, bp::std_in < pipe);
    test(true, pipe.native_sink()  , EXIT_FAILURE, bp::std_in < pipe);

    test(false, STDOUT_FILENO       , EXIT_SUCCESS, bp::std_out > pipe);
    test(false, pipe.native_source(), EXIT_FAILURE, bp::std_out > pipe);
    test(false, pipe.native_sink()  , EXIT_FAILURE, bp::std_out > pipe);

    test(false, STDERR_FILENO       , EXIT_SUCCESS, bp::std_err > pipe);
    test(false, pipe.native_source(), EXIT_FAILURE, bp::std_err > pipe);
    test(false, pipe.native_sink()  , EXIT_FAILURE, bp::std_err > pipe);

    test(false, STDOUT_FILENO       , EXIT_SUCCESS, (bp::std_out & bp::std_err) > pipe);
    test(false, STDERR_FILENO       , EXIT_SUCCESS, (bp::std_out & bp::std_err) > pipe);
    test(false, pipe.native_source(), EXIT_FAILURE, (bp::std_out & bp::std_err) > pipe);
    test(false, pipe.native_sink()  , EXIT_FAILURE, (bp::std_out & bp::std_err) > pipe);

    #undef test
}
