// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <functional>
#include <limits>
#include <string>
#include <type_traits>

using Asc  = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;
using Desc = eggs::flat_keyed_set<test::Employee, &test::Employee::id, std::greater<int>>;

TEST_CASE("flat_keyed_set — default Compare is std::less<key_type>", "[flat_keyed_set.compare]")
{
    static_assert(std::is_same_v<Asc::key_compare, std::less<int>>);
}

TEST_CASE("flat_keyed_set — default Compare iterates in ascending order",
          "[flat_keyed_set.compare]")
{
    Asc m{{3, "C"}, {1, "A"}, {2, "B"}};
    int prev = -1;
    for (auto const& e : m) { CHECK(e.id > prev); prev = e.id; }
}

TEST_CASE("flat_keyed_set — std::greater<> yields descending order",
          "[flat_keyed_set.compare]")
{
    Desc m{{3, "C"}, {1, "A"}, {2, "B"}};
    int prev = std::numeric_limits<int>::max();
    for (auto const& e : m) { CHECK(e.id < prev); prev = e.id; }
}

TEST_CASE("flat_keyed_set — key_comp() returns the Compare object",
          "[flat_keyed_set.compare]")
{
    Desc m;
    auto cmp = m.key_comp();
    static_assert(std::is_same_v<decltype(cmp), std::greater<int>>);
    CHECK(cmp(3, 2));
    CHECK(!cmp(2, 3));
}

TEST_CASE("flat_keyed_set — non-transparent key_compare has no is_transparent",
          "[flat_keyed_set.compare]")
{
    static_assert(!test::transparent<Asc::key_compare>);
    static_assert(test::transparent<Asc::value_compare>);
}

TEST_CASE("flat_keyed_set — std::less<void> propagates is_transparent",
          "[flat_keyed_set.compare]")
{
    using Trans = eggs::flat_keyed_set<test::Employee, &test::Employee::id, std::less<>>;
    static_assert(test::transparent<Trans::key_compare>);
}

#endif // __cpp_lib_flat_set
