// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <vector>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("unordered_keyed_set(i, j) — range constructor", "[unordered_keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}};
    M m(src.begin(), src.end());
    REQUIRE(m.size() == 3u);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
    CHECK(m.contains(3));
}

TEST_CASE("unordered_keyed_set(i, j) — duplicate keys deduplicated",
          "[unordered_keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {1, "Dup"}};
    M m(src.begin(), src.end());
    CHECK(m.size() == 2u);
}
