// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <functional>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("find(key_type) — present key", "[unordered_keyed_set.lookup]")
{
    M m{{1,"Alice"},{2,"Bob"}};
    auto it = m.find(1);
    REQUIRE(it != m.end());
    CHECK(it->name == "Alice");
}

TEST_CASE("find(key_type) — absent key returns end()", "[unordered_keyed_set.lookup]")
{
    M m{{1,"A"}};
    CHECK(m.find(99) == m.end());
}

TEST_CASE("find — transparent overload with std::hash<>/equal_to<>",
          "[unordered_keyed_set.lookup]")
{
    using Trans = eggs::unordered_keyed_set<
        test::Employee, &test::Employee::id,
        std::hash<int>, std::equal_to<>>;
    Trans m{{1,"A"},{2,"B"}};
    auto it = m.find(1L);  // long, not int
    REQUIRE(it != m.end());
    CHECK(it->id == 1);
}
