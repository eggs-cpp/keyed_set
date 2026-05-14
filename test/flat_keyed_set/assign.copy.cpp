// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("operator=(const flat_keyed_set&) — copy assignment", "[flat_keyed_set.assign]")
{
    M src{{1, "Alice"}, {2, "Bob"}};
    M dst{{99, "Old"}};
    dst = src;
    CHECK(dst == src);
}

TEST_CASE("operator=(flat_keyed_set&&) — move assignment", "[flat_keyed_set.assign]")
{
    M src{{1, "Alice"}, {2, "Bob"}};
    M dst;
    dst = std::move(src);
    CHECK(dst.size() == 2u);
    CHECK(dst.contains(1));
}

TEST_CASE("operator=(initializer_list) — init-list assignment", "[flat_keyed_set.assign]")
{
    M m{{1, "Old"}};
    m = {{10, "X"}, {20, "Y"}};
    REQUIRE(m.size() == 2u);
    CHECK(m.contains(10));
    CHECK(!m.contains(1));
}

#endif // __cpp_lib_flat_set
