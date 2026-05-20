// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("unordered_keyed_set(const&) — copy constructor",
          "[unordered_keyed_set.cnstr]")
{
    M src{{1, "A"}, {2, "B"}};
    M copy(src);
    CHECK(copy == src);
    CHECK(copy.size() == 2u);
}

TEST_CASE("unordered_keyed_set(&&) — move constructor",
          "[unordered_keyed_set.cnstr]")
{
    M src{{1, "A"}, {2, "B"}};
    M moved(std::move(src));
    CHECK(moved.size() == 2u);
    CHECK(moved.contains(1));
}
