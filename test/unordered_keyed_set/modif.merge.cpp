// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>
#include <catch.hpp>
#include "../fixture.hpp"

using M = eggs::unordered_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("merge(M&) — transfers non-conflicting elements",
          "[unordered_keyed_set.modif]")
{
    M a{{1,"A"},{2,"B"}};
    M b{{3,"C"},{4,"D"}};
    a.merge(b);
    CHECK(a.size() == 4u);
    CHECK(b.empty());
}

TEST_CASE("merge(M&) — duplicate keys stay in source",
          "[unordered_keyed_set.modif]")
{
    M a{{1,"A"},{2,"B"}};
    M b{{2,"Dup"},{3,"C"}};
    a.merge(b);
    CHECK(a.size() == 3u);
    CHECK(b.size() == 1u);
    CHECK(b.contains(2));
    CHECK(a.find(2)->name == "B");
}
