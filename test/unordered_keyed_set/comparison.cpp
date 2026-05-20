// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("operator== — equal containers", "[unordered_keyed_set.cmp]")
{
    M a{{1,"Alice"},{2,"Bob"}};
    M b{{2,"Bob"},{1,"Alice"}};  // same elements, different insertion order
    CHECK(a == b);
}

TEST_CASE("operator== — different sizes", "[unordered_keyed_set.cmp]")
{
    M a{{1,"A"}};
    M b{{1,"A"},{2,"B"}};
    CHECK(!(a == b));
}

TEST_CASE("operator== — same keys different values are not equal",
          "[unordered_keyed_set.cmp]")
{
    M a{{1,"Alice"}};
    M b{{1,"Bob"}};
    CHECK(!(a == b));
}

template <typename C>
concept has_spaceship = requires (C const& a, C const& b) { a <=> b; };

TEST_CASE("unordered_keyed_set — no operator<=>", "[unordered_keyed_set.cmp]")
{
    static_assert(!has_spaceship<M>);
}
