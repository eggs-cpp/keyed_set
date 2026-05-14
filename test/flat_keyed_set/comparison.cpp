// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <compare>

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("operator== — equal containers", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{1, "Alice"}, {2, "Bob"}};
    CHECK(a == b);
}

TEST_CASE("operator== — different size", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Alice"}, {2, "Bob"}};
    CHECK(!(a == b));
}

TEST_CASE("operator<=> — less than", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}};
    CHECK(std::is_lt(a <=> b));
    CHECK(std::is_gt(b <=> a));
}

TEST_CASE("operator<=> — equal", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Alice"}};
    CHECK(std::is_eq(a <=> b));
}

TEST_CASE("operator<= / >= / < / > — synthesised from <=>", "[flat_keyed_set.cmp]")
{
    M a{{1, "A"}};
    M b{{2, "B"}};
    CHECK(a < b);
    CHECK(a <= b);
    CHECK(b > a);
    CHECK(b >= a);
}

TEST_CASE("operator== — same keys different values are not equal", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Bob"}};
    CHECK(!(a == b));
}

TEST_CASE("operator<=> — full value comparison, not just key", "[flat_keyed_set.cmp]")
{
    M a{{1, "Alice"}};
    M b{{1, "Bob"}};
    CHECK(std::is_lt(a <=> b));
}

#endif // __cpp_lib_flat_set
