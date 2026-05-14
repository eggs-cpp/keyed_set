// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <array>
#include <vector>

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("flat_keyed_set(i, j) — iterator-range constructor", "[flat_keyed_set.cnstr]")
{
    std::array<test::Employee, 3> elems{{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}}};
    M m(elems.begin(), elems.end());

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
    CHECK(m.contains(3));
}

TEST_CASE("flat_keyed_set(sorted_unique, i, j) — sorted range, skips sort step",
          "[flat_keyed_set.cnstr]")
{
    std::vector<test::Employee> sorted{{1, "A"}, {2, "B"}, {3, "C"}};
    M m(std::sorted_unique, sorted.begin(), sorted.end());

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(1));
    CHECK(m.contains(3));
}

TEST_CASE("flat_keyed_set(i, j) — duplicates deduplicated", "[flat_keyed_set.cnstr]")
{
    std::array<test::Employee, 4> elems{{{1, "A"}, {2, "B"}, {1, "Dup"}, {3, "C"}}};
    M m(elems.begin(), elems.end());

    CHECK(m.size() == 3u);
}

#endif // __cpp_lib_flat_set
