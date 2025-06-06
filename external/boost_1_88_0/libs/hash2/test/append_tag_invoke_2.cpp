// Copyright 2024 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/fnv1a.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>

class X
{
private:

    std::string s1_;
    std::string s2_;

    template<class Provider, class Hash, class Flavor>
    friend void tag_invoke( boost::hash2::hash_append_tag const&, Provider const&, Hash& h, Flavor const& f, X const* x )
    {
        boost::hash2::hash_append( h, f, x->s1_ );
        boost::hash2::hash_append( h, f, x->s2_ );
    }

public:

    X(): s1_( "s1" ), s2_( "s2" )
    {
    }

    std::string s1() const
    {
        return s1_;
    }

    std::string s2() const
    {
        return s2_;
    }
};

template<class Hash, class Flavor, class R> void test( X const& x, R r )
{
    Flavor f;

    {
        Hash h;
        hash_append( h, f, x );

        BOOST_TEST_EQ( h.result(), r );
    }

    {
        Hash h;
        hash_append( h, f, x.s1() );
        hash_append( h, f, x.s2() );

        BOOST_TEST_EQ( h.result(), r );
    }
}

int main()
{
    using namespace boost::hash2;

    test<fnv1a_32, little_endian_flavor>( {}, 1388033884 );
    test<fnv1a_32, big_endian_flavor>( {}, 3611226160 );

    return boost::report_errors();
}
