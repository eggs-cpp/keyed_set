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

TEST_CASE("insert_range(rg)", "[unordered_keyed_set.modif]")
{
    std::vector<test::Employee> v{{1,"A"},{2,"B"},{3,"C"}};
    M m;
    m.insert_range(v);
    CHECK(m.size() == 3u);
}

TEST_CASE("insert_range — duplicates ignored", "[unordered_keyed_set.modif]")
{
    M m{{1, "Existing"}};
    std::vector<test::Employee> v{{1,"Dup"},{2,"New"}};
    m.insert_range(v);
    CHECK(m.size() == 2u);
    CHECK(m.find(1)->name == "Existing");
}

#else

TEST_CASE("insert_range — not available", "[unordered_keyed_set.modif]")
{
    SUCCEED("skipped");
}

#endif
