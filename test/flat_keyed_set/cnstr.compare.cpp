// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

TEST_CASE("flat_keyed_set(c) — comparator constructor", "[flat_keyed_set.cnstr]")
{
    using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;
    M m(std::less<int>{});
    CHECK(m.empty());
}

#endif // __cpp_lib_flat_set
