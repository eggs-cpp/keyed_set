// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>

#include <catch.hpp>

#include "../fixture.hpp"

#include <functional>
#include <iterator>
#include <type_traits>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("unordered_keyed_set — key_type", "[unordered_keyed_set.types]")
{
    static_assert(std::is_same_v<M::key_type, int>);
}

TEST_CASE("unordered_keyed_set — value_type", "[unordered_keyed_set.types]")
{
    static_assert(std::is_same_v<M::value_type, test::Employee>);
}

TEST_CASE("unordered_keyed_set — hasher is the user-supplied Hash",
          "[unordered_keyed_set.types]")
{
    static_assert(std::is_same_v<M::hasher, std::hash<int>>);
}

TEST_CASE("unordered_keyed_set — key_equal is the user-supplied KeyEqual",
          "[unordered_keyed_set.types]")
{
    static_assert(std::is_same_v<M::key_equal, std::equal_to<int>>);
}

TEST_CASE("unordered_keyed_set — iterator is constant",
          "[unordered_keyed_set.types]")
{
    static_assert(std::is_same_v<M::iterator, M::const_iterator>);
    static_assert(std::is_const_v<
        std::remove_reference_t<decltype(*std::declval<M::iterator>())>>);
}

template <typename C>
concept has_lower_bound = requires (C const& c) { c.lower_bound(0); };
template <typename C>
concept has_rbegin = requires (C const& c) { c.rbegin(); };

TEST_CASE("unordered_keyed_set — no ordering (no lower_bound etc.)",
          "[unordered_keyed_set.types]")
{
    static_assert(!has_lower_bound<M>);
    static_assert(!has_rbegin<M>);
}

TEST_CASE("unordered_keyed_set — has node_type", "[unordered_keyed_set.types]")
{
    static_assert(requires { typename M::node_type; });
}
