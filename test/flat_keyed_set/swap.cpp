// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("swap(flat_keyed_set&) — member swap", "[flat_keyed_set.swap]")
{
    M a{{1, "Alice"}, {2, "Bob"}};
    M b{{10, "X"}};

    a.swap(b);

    CHECK(a.size() == 1u);
    CHECK(a.contains(10));
    CHECK(b.size() == 2u);
    CHECK(b.contains(1));
}

TEST_CASE("swap(flat_keyed_set&, flat_keyed_set&) — non-member ADL swap",
          "[flat_keyed_set.swap]")
{
    M a{{1, "Alice"}};
    M b{{2, "Bob"}, {3, "Carol"}};

    using std::swap;
    swap(a, b);

    CHECK(a.size() == 2u);
    CHECK(b.size() == 1u);
}

#endif // __cpp_lib_flat_set
