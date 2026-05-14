// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <ranges>
#include <vector>

#ifdef __cpp_lib_ranges_to_container
using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("flat_keyed_set(from_range, rg)", "[flat_keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{10, "Dave"}, {20, "Eve"}, {30, "Frank"}};
    M m(std::from_range, src);

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(10));
    CHECK(m.contains(20));
    CHECK(m.contains(30));
}

#else

TEST_CASE("flat_keyed_set(from_range) — not available on this compiler",
          "[flat_keyed_set.cnstr]")
{
    SUCCEED("__cpp_lib_ranges_to_container is not defined; from_range tests skipped");
}

#endif

#endif // __cpp_lib_flat_set
