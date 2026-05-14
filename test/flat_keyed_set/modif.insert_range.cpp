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

TEST_CASE("insert_range(rg) — inserts all elements", "[flat_keyed_set.modif]")
{
    std::vector<test::Employee> src{{1, "A"}, {2, "B"}, {3, "C"}};
    M m;
    m.insert_range(src);
    CHECK(m.size() == 3u);
}

TEST_CASE("insert_range(rg) — duplicates ignored", "[flat_keyed_set.modif]")
{
    M m{{1, "Existing"}};
    std::vector<test::Employee> src{{1, "Dup"}, {2, "New"}};
    m.insert_range(src);
    CHECK(m.size() == 2u);
    CHECK(m.find(1)->name == "Existing");
}

TEST_CASE("insert_range(sorted_unique, rg) — skips sort, merges with existing",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {3, "C"}, {5, "E"}};
    std::vector<test::Employee> sorted{{2, "B"}, {4, "D"}};

    m.insert_range(eggs::sorted_unique, sorted);

    REQUIRE(m.size() == 5u);
    int prev = -1;
    for (auto const& e : m) { CHECK(e.id > prev); prev = e.id; }
}

TEST_CASE("insert_range(sorted_unique, rg) — duplicates not inserted",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}};
    std::vector<test::Employee> sorted{{2, "Dup"}, {3, "C"}};

    m.insert_range(eggs::sorted_unique, sorted);

    CHECK(m.size() == 3u);
    CHECK(m.find(2)->name == "B");
}

#else

TEST_CASE("insert_range — not available on this compiler", "[flat_keyed_set.modif]")
{
    SUCCEED("__cpp_lib_ranges_to_container is not defined; insert_range tests skipped");
}

#endif

#endif // __cpp_lib_flat_set
