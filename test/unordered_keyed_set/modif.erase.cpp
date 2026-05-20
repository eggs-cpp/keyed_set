// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("erase(key_type) — removes by key", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"},{2,"B"},{3,"C"}};
    CHECK(m.erase(2) == 1u);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}

TEST_CASE("erase(key_type) — missing key returns 0", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"}};
    CHECK(m.erase(99) == 0u);
    CHECK(m.size() == 1u);
}

TEST_CASE("erase(const_iterator)", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"},{2,"B"}};
    auto it = m.find(1);
    REQUIRE(it != m.end());
    m.erase(it);
    CHECK(m.size() == 1u);
    CHECK(!m.contains(1));
}

TEST_CASE("erase(q1, q2) — range erase", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"},{2,"B"},{3,"C"}};
    m.erase(m.begin(), m.end());
    CHECK(m.empty());
}
