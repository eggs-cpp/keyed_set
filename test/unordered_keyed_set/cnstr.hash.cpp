// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"
#include <functional>

TEST_CASE("unordered_keyed_set(n, h, eq) — bucket_count + hash + eq constructor",
          "[unordered_keyed_set.cnstr]")
{
    eggs::unordered_keyed_set<test::Employee, &test::Employee::id> m(
        16, std::hash<int>{}, std::equal_to<int>{});
    CHECK(m.empty());
    CHECK(m.bucket_count() >= 16u);
}
