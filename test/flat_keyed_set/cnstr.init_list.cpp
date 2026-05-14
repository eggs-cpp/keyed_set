// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("flat_keyed_set(il) — initializer-list constructor", "[flat_keyed_set.cnstr]")
{
    M m{{1, "Alice"}, {2, "Bob"}, {3, "Carol"}};

    REQUIRE(m.size() == 3u);
    CHECK(m.find(1)->name == "Alice");
    CHECK(m.find(3)->name == "Carol");
}

TEST_CASE("flat_keyed_set(sorted_unique, il) — sorted init-list", "[flat_keyed_set.cnstr]")
{
    M m(std::sorted_unique, {{1, "A"}, {2, "B"}, {3, "C"}});

    REQUIRE(m.size() == 3u);
    CHECK(m.contains(2));
}

#endif // __cpp_lib_flat_set
