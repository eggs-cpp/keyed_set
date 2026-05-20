// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("contains(key_type) — present", "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"},{2,"B"}};
    CHECK(m.contains(1));
    CHECK(m.contains(2));
    CHECK(!m.contains(3));
}
