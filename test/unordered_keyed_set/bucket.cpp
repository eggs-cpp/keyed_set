// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("bucket_count() — positive after construction", "[unordered_keyed_set.bucket]")
{
    M m{{1,"A"},{2,"B"}};
    CHECK(m.bucket_count() > 0u);
    CHECK(m.max_bucket_count() >= m.bucket_count());
}

TEST_CASE("load_factor() — between 0 and max_load_factor()",
          "[unordered_keyed_set.bucket]")
{
    M m{{1,"A"},{2,"B"},{3,"C"}};
    CHECK(m.load_factor() > 0.0f);
    CHECK(m.load_factor() <= m.max_load_factor());
}

TEST_CASE("reserve(n) — bucket_count sufficient for n elements",
          "[unordered_keyed_set.bucket]")
{
    M m;
    m.reserve(100);
    CHECK(m.bucket_count() >= 1u);
}

TEST_CASE("rehash(n) — bucket_count >= n", "[unordered_keyed_set.bucket]")
{
    M m{{1,"A"},{2,"B"}};
    m.rehash(64);
    CHECK(m.bucket_count() >= 64u);
    CHECK(m.contains(1));
    CHECK(m.contains(2));
}

TEST_CASE("max_load_factor(f) — sets the limit", "[unordered_keyed_set.bucket]")
{
    M m;
    m.max_load_factor(0.5f);
    CHECK(m.max_load_factor() == Approx(0.5f));
}
