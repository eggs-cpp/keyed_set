// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <iterator>
#include <type_traits>

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("flat_keyed_set — key_type", "[flat_keyed_set.types]")
{
    static_assert(std::is_same_v<M::key_type, int>);
}

TEST_CASE("flat_keyed_set — value_type", "[flat_keyed_set.types]")
{
    static_assert(std::is_same_v<M::value_type, test::Employee>);
}

TEST_CASE("flat_keyed_set — key_compare is Compare", "[flat_keyed_set.types]")
{
    static_assert(std::is_same_v<M::key_compare, std::less<int>>);
}

TEST_CASE("flat_keyed_set — value_compare is the projecting adaptor", "[flat_keyed_set.types]")
{
    static_assert(!std::is_same_v<M::key_compare, M::value_compare>);
}

TEST_CASE("flat_keyed_set — iterator is constant random-access", "[flat_keyed_set.types]")
{
    static_assert(std::is_same_v<M::iterator, M::const_iterator>);
    static_assert(std::random_access_iterator<M::iterator>);
    static_assert(std::is_const_v<std::remove_reference_t<decltype(*std::declval<M::iterator>())>>);
}

TEST_CASE("flat_keyed_set — no node_type", "[flat_keyed_set.types]")
{
    static_assert(!requires { typename M::node_type; });
    static_assert(!requires { typename M::insert_return_type; });
}

TEST_CASE("flat_keyed_set — has container_type", "[flat_keyed_set.types]")
{
    static_assert(requires { typename M::container_type; });
    static_assert(std::is_same_v<M::container_type, std::vector<test::Employee>>);
}

#endif // __cpp_lib_flat_set
