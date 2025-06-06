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
#include <boost/system/error_code.hpp>
#include <boost/process/v1/args.hpp>
#include <boost/process/v1/child.hpp>
#include <boost/process/v1/exe.hpp>
#include <boost/process/v1/filesystem.hpp>
#include <boost/process/v1/io.hpp>
#include <boost/process/v1/pipe.hpp>
#include <boost/process/v1/start_dir.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <string>

namespace bp = boost::process::v1;


struct test_dir
{
    std::string s_;
    test_dir(const std::string &s) : s_(s)
    { BOOST_REQUIRE_NO_THROW(boost::process::v1::filesystem::create_directory(s)); }
    ~test_dir() { boost::process::v1::filesystem::remove(s_); }
};

BOOST_AUTO_TEST_CASE(start_in_dir)
{
    using boost::unit_test::framework::master_test_suite;

    test_dir dir("start_in_dir_test");

    bp::ipstream is;

    std::error_code ec;
    bp::child c(
        bp::exe=boost::process::v1::filesystem::absolute(master_test_suite().argv[1]).string(),
        bp::args +={"test", "--pwd"},
        bp::start_dir = dir.s_,
        bp::std_out>is,
        ec
    );
    BOOST_REQUIRE(!ec);


    std::string s;
    std::getline(is, s);
    auto path_read = boost::process::v1::filesystem::absolute(boost::process::v1::filesystem::path(s)).string();
    auto path_set  = boost::process::v1::filesystem::absolute(dir.s_).string();

    if (path_read.size() > path_set.size())
        path_read.resize(path_set.size());
    else if (path_read.size() < path_set.size())
        path_set.resize(path_read.size());

    BOOST_CHECK_EQUAL_COLLECTIONS(path_read.begin(), path_read.end(),
                                  path_set.begin(), path_set.end());

    BOOST_REQUIRE_NO_THROW(c.wait());
}
