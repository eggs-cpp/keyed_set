// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <vector>

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("insert(value_type const&)", "[unordered_keyed_set.modif]")
{
    M m;
    test::Employee e{1, "Alice"};
    auto [it, ok] = m.insert(e);
    REQUIRE(ok);
    CHECK(it->id == 1);
}

TEST_CASE("insert(value_type&&)", "[unordered_keyed_set.modif]")
{
    M m;
    auto [it, ok] = m.insert(test::Employee{2, "Bob"});
    REQUIRE(ok);
    CHECK(it->id == 2);
}

TEST_CASE("insert(i, j)", "[unordered_keyed_set.modif]")
{
    std::vector<test::Employee> v{{1,"A"},{2,"B"},{3,"C"}};
    M m;
    m.insert(v.begin(), v.end());
    CHECK(m.size() == 3u);
}

TEST_CASE("insert(il)", "[unordered_keyed_set.modif]")
{
    M m;
    m.insert({{1,"A"},{2,"B"}});
    CHECK(m.size() == 2u);
}

TEST_CASE("insert — duplicate rejected", "[unordered_keyed_set.modif]")
{
    M m{{1, "Alice"}};
    auto [it, ok] = m.insert({1, "Dup"});
    CHECK(!ok);
    CHECK(it->name == "Alice");
}
