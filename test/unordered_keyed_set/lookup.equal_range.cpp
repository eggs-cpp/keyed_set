// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <iterator>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("equal_range — present key returns range of one",
          "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"},{2,"B"}};
    auto [lo, hi] = m.equal_range(1);
    REQUIRE(lo != m.end());
    CHECK(lo->id == 1);
    CHECK(std::distance(lo, hi) == 1);
}

TEST_CASE("equal_range — absent key returns [end, end)",
          "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"}};
    auto [lo, hi] = m.equal_range(99);
    CHECK(lo == m.end());
    CHECK(hi == m.end());
}
