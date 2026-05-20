// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <functional>
#include <type_traits>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("hash_function() — returns the Hash object", "[unordered_keyed_set.obs]")
{
    M m;
    auto h = m.hash_function();
    static_assert(std::is_same_v<decltype(h), M::hasher>);
    CHECK(h(1) == h(1));
}

TEST_CASE("key_eq() — returns the KeyEqual object", "[unordered_keyed_set.obs]")
{
    M m;
    auto eq = m.key_eq();
    static_assert(std::is_same_v<decltype(eq), M::key_equal>);
    CHECK(eq(1, 1));
    CHECK(!eq(1, 2));
}

TEST_CASE("hasher is Compare, not the projecting adaptor",
          "[unordered_keyed_set.obs]")
{
    static_assert(std::is_same_v<M::hasher, std::hash<int>>);
    static_assert(!test::transparent<M::hasher>);
}
