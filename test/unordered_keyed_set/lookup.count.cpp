// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("count(key_type) — present key returns 1", "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"}};
    CHECK(m.count(1) == 1u);
}

TEST_CASE("count(key_type) — absent key returns 0", "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"}};
    CHECK(m.count(99) == 0u);
}
