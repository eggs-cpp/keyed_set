// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <algorithm>
#include <vector>

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("keys() — returns const reference to underlying container",
          "[flat_keyed_set.sequence]")
{
    M m{{3, "C"}, {1, "A"}, {2, "B"}};

    auto const& keys = m.keys();
    static_assert(std::is_same_v<decltype(keys), std::vector<test::Employee> const&>);

    // Elements are in sorted order
    REQUIRE(keys.size() == 3u);
    CHECK(keys[0].id == 1);
    CHECK(keys[1].id == 2);
    CHECK(keys[2].id == 3);
}

TEST_CASE("keys() — provides random-access to sorted elements",
          "[flat_keyed_set.sequence]")
{
    M m{{10, "X"}, {20, "Y"}, {30, "Z"}};

    auto const& keys = m.keys();
    CHECK(keys[0].id == 10);
    CHECK(keys[1].id == 20);
    CHECK(keys[2].id == 30);

    // Random-access distance
    CHECK(keys.back().id == 30);
}

TEST_CASE("extract_sequence() — moves out the underlying container",
          "[flat_keyed_set.sequence]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};

    auto cont = std::move(m).extract_sequence();

    static_assert(std::is_same_v<decltype(cont), std::vector<test::Employee>>);
    REQUIRE(cont.size() == 3u);
    CHECK(cont[0].id == 1);
    CHECK(cont[1].id == 2);
    CHECK(cont[2].id == 3);
}

TEST_CASE("replace_sequence() — replaces the underlying container",
          "[flat_keyed_set.sequence]")
{
    M m{{1, "A"}, {2, "B"}};

    std::vector<test::Employee> new_cont{{10, "X"}, {20, "Y"}, {30, "Z"}};
    m.replace_sequence(std::move(new_cont));

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(10));
    CHECK(m.contains(20));
    CHECK(m.contains(30));
    CHECK(!m.contains(1));
}

TEST_CASE("extract_sequence / replace_sequence — round-trip",
          "[flat_keyed_set.sequence]")
{
    M m{{5, "E"}, {3, "C"}, {1, "A"}};

    // Extract, modify, replace
    auto cont = std::move(m).extract_sequence();
    cont.push_back({7, "G"});
    std::sort(cont.begin(), cont.end(),
        [](test::Employee const& a, test::Employee const& b){ return a.id < b.id; });

    m.replace_sequence(std::move(cont));

    REQUIRE(m.size() == 4u);
    CHECK(m.contains(7));
    // Still sorted
    int prev = -1;
    for (auto const& e : m) { CHECK(e.id > prev); prev = e.id; }
}

#endif // __cpp_lib_flat_set
