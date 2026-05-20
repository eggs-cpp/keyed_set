// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("erase_if — removes matching elements", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"},{2,"B"},{3,"C"},{4,"D"}};
    auto n = eggs::erase_if(m, [](test::Employee const& e){ return e.id % 2 == 0; });
    CHECK(n == 2u);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
    CHECK(!m.contains(4));
}

TEST_CASE("erase_if — no match returns 0", "[unordered_keyed_set.modif]")
{
    M m{{1,"A"},{2,"B"}};
    CHECK(eggs::erase_if(m, [](test::Employee const& e){ return e.id > 99; }) == 0u);
    CHECK(m.size() == 2u);
}
