// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <catch.hpp>

#ifdef __cpp_lib_ranges_to_container

#include <eggs/unordered_keyed_set.hpp>
#include "../fixture.hpp"
#include <ranges>
#include <vector>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("unordered_keyed_set(from_range, rg)", "[unordered_keyed_set.cnstr]")
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}};
    M m(std::from_range, src);
    REQUIRE(m.size() == 3u);
    CHECK(m.contains(1));
}

#else

TEST_CASE("unordered_keyed_set(from_range) — not available on this compiler",
          "[unordered_keyed_set.cnstr]")
{
    SUCCEED("__cpp_lib_ranges_to_container not defined; skipped");
}

#endif
