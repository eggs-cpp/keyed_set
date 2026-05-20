// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("operator=(const&) — copy assignment", "[unordered_keyed_set.assign]")
{
    M a{{1, "A"}, {2, "B"}};
    M b;
    b = a;
    CHECK(b == a);
}

TEST_CASE("operator=(&&) — move assignment", "[unordered_keyed_set.assign]")
{
    M a{{1, "A"}, {2, "B"}};
    M b;
    b = std::move(a);
    CHECK(b.size() == 2u);
}

TEST_CASE("operator=(il) — init-list assignment", "[unordered_keyed_set.assign]")
{
    M m{{99, "Old"}};
    m = {{1, "A"}, {2, "B"}};
    REQUIRE(m.size() == 2u);
    CHECK(!m.contains(99));
    CHECK(m.contains(1));
}
