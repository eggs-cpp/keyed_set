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

TEST_CASE("insert(value_type const&)", "[flat_keyed_set.modif]")
{
    M m;
    test::Employee e{42, "Alice"};
    auto [it, ok] = m.insert(e);
    REQUIRE(ok);
    CHECK(it->id == 42);
}

TEST_CASE("insert(value_type&&)", "[flat_keyed_set.modif]")
{
    M m;
    auto [it, ok] = m.insert(test::Employee{7, "Bob"});
    REQUIRE(ok);
    CHECK(it->id == 7);
}

TEST_CASE("insert(i, j)", "[flat_keyed_set.modif]")
{
    std::array<test::Employee, 3> arr{{{10, "X"}, {20, "Y"}, {30, "Z"}}};
    M m;
    m.insert(arr.begin(), arr.end());
    CHECK(m.size() == 3u);
}

TEST_CASE("insert(sorted_unique, i, j) — skips sort step", "[flat_keyed_set.modif]")
{
    std::vector<test::Employee> sorted{{1, "A"}, {2, "B"}, {3, "C"}};
    M m;
    m.insert(std::sorted_unique, sorted.begin(), sorted.end());
    CHECK(m.size() == 3u);
}

TEST_CASE("insert(il)", "[flat_keyed_set.modif]")
{
    M m;
    m.insert({{1, "A"}, {2, "B"}, {3, "C"}});
    CHECK(m.size() == 3u);
}

TEST_CASE("insert — duplicate key rejected", "[flat_keyed_set.modif]")
{
    M m{{1, "Alice"}};
    auto [it, ok] = m.insert({1, "Dup"});
    CHECK(!ok);
    CHECK(it->name == "Alice");
    CHECK(m.size() == 1u);
}

#endif // __cpp_lib_flat_set
