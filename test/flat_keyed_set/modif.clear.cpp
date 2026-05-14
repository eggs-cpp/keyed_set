// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

TEST_CASE("clear() — postcondition: empty()", "[flat_keyed_set.modif]")
{
    eggs::flat_keyed_set<test::Employee, &test::Employee::id> m{
        {1, "A"}, {2, "B"}, {3, "C"}};
    m.clear();
    CHECK(m.empty());
    CHECK(m.begin() == m.end());
}

#endif // __cpp_lib_flat_set
