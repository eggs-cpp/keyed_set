// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

TEST_CASE("clear() — postcondition: empty()", "[unordered_keyed_set.modif]")
{
    eggs::unordered_keyed_set<test::Employee, &test::Employee::id> m{
        {1,"A"},{2,"B"}};
    m.clear();
    CHECK(m.empty());
}
