// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("swap — member swap", "[unordered_keyed_set.swap]")
{
    M a{{1,"A"},{2,"B"}};
    M b{{10,"X"}};
    a.swap(b);
    CHECK(a.size() == 1u);
    CHECK(a.contains(10));
    CHECK(b.size() == 2u);
    CHECK(b.contains(1));
}

TEST_CASE("swap — non-member ADL swap", "[unordered_keyed_set.swap]")
{
    M a{{1,"A"}};
    M b{{2,"B"},{3,"C"}};
    using std::swap;
    swap(a, b);
    CHECK(a.size() == 2u);
    CHECK(b.size() == 1u);
}
