// Copyright Agustin K-ballo Berge, Fusion Fenix 2025
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("contains(key_type const&) — present", "[flat_keyed_set.lookup]")
{
    M m{{1, "Alice"}};
    CHECK(m.contains(1));
}

TEST_CASE("contains(key_type const&) — absent", "[flat_keyed_set.lookup]")
{
    M m{{1, "Alice"}};
    CHECK(!m.contains(99));
}

TEST_CASE("contains(K const&) — transparent overload", "[flat_keyed_set.lookup]")
{
    M m{{7, "G"}};
    CHECK(m.contains(static_cast<long>(7)));
    CHECK(!m.contains(static_cast<long>(8)));
}

TEST_CASE("contains — equivalent to find() != end()", "[flat_keyed_set.lookup]")
{
    M m{{3, "C"}, {4, "D"}};
    for (int k : {3, 4, 5})
        CHECK(m.contains(k) == (m.find(k) != m.end()));
}

#endif // __cpp_lib_flat_set
