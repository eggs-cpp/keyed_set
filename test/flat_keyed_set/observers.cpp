// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

#include <functional>
#include <type_traits>

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("key_comp() — returns the Compare object", "[flat_keyed_set.obs]")
{
    M m;
    auto cmp = m.key_comp();
    static_assert(std::is_same_v<decltype(cmp), M::key_compare>);
    static_assert(std::is_same_v<M::key_compare, std::less<int>>);
    CHECK(cmp(1, 2));
    CHECK(!cmp(2, 1));
}

TEST_CASE("value_comp() — returns the projecting comparator", "[flat_keyed_set.obs]")
{
    M m;
    static_assert(!std::is_same_v<M::key_compare, M::value_compare>);
    auto vc = m.value_comp();
    test::Employee a{1, "A"}, b{2, "B"};
    CHECK(vc(a, b));
    CHECK(!vc(b, a));
}

TEST_CASE("key_compare::is_transparent — present only when Compare is transparent",
          "[flat_keyed_set.obs]")
{
    static_assert(!test::transparent<M::key_compare>);

    using Trans = eggs::flat_keyed_set<test::Employee, &test::Employee::id, std::less<>>;
    static_assert(test::transparent<Trans::key_compare>);
}

TEST_CASE("value_compare::is_transparent — always present", "[flat_keyed_set.obs]")
{
    static_assert(test::transparent<M::value_compare>);
}

#endif // __cpp_lib_flat_set
