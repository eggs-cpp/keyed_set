// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("emplace() — inserts new element", "[flat_keyed_set.modif]")
{
    M m;
    auto [it, ok] = m.emplace(test::Employee{1, "Alice"});
    REQUIRE(ok);
    CHECK(it->id == 1);
}

TEST_CASE("emplace() — duplicate key is rejected", "[flat_keyed_set.modif]")
{
    M m{{1, "Alice"}};
    auto [it, ok] = m.emplace(test::Employee{1, "Dup"});
    CHECK(!ok);
    CHECK(it->name == "Alice");
    CHECK(m.size() == 1u);
}

TEST_CASE("emplace_hint() — inserts near hint", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {3, "C"}};
    auto it = m.emplace_hint(m.end(), test::Employee{2, "B"});
    CHECK(it->id == 2);
    CHECK(m.size() == 3u);
}

TEST_CASE("emplace_hint() — wrong hint still produces correct result",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto it = m.emplace_hint(m.begin(), test::Employee{5, "E"});
    CHECK(it->id == 5);
    CHECK(m.size() == 4u);
    int prev = -1;
    for (auto const& e : m) { CHECK(e.id > prev); prev = e.id; }
}

#endif // __cpp_lib_flat_set
