// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/flat_keyed_set.hpp>

#ifdef __cpp_lib_flat_set
#include <catch.hpp>

#include "../fixture.hpp"

using M = eggs::flat_keyed_set<test::Employee, &test::Employee::id>;

TEST_CASE("erase(key_type const&) — removes by key", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto n = m.erase(2);
    CHECK(n == 1u);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
}

TEST_CASE("erase(key_type const&) — missing key returns 0", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}};
    CHECK(m.erase(99) == 0u);
    CHECK(m.size() == 1u);
}

TEST_CASE("erase(key_type const&) — only element leaves empty container",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}};
    m.erase(1);
    CHECK(m.empty());
}

TEST_CASE("erase(const_iterator) — removes by iterator, returns next",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto next = m.erase(m.find(2));
    CHECK(next->id == 3);
    CHECK(m.size() == 2u);
}

TEST_CASE("erase(const_iterator) — erase at begin() returns new begin()",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto next = m.erase(m.begin());
    CHECK(next == m.begin());
    CHECK(next->id == 2);
}

TEST_CASE("erase(const_iterator) — erase last element returns end()",
          "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}};
    auto next = m.erase(m.find(2));
    CHECK(next == m.end());
}

TEST_CASE("erase(q1, q2) — range erase", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}};
    auto after = m.erase(m.find(2), m.find(4));
    CHECK(after->id == 4);
    CHECK(m.size() == 2u);
    CHECK(!m.contains(2));
    CHECK(!m.contains(3));
}

TEST_CASE("erase(q1, q2) — erase all clears container", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}, {3, "C"}};
    auto after = m.erase(m.begin(), m.end());
    CHECK(after == m.end());
    CHECK(m.empty());
}

TEST_CASE("erase(K&&) — transparent erase", "[flat_keyed_set.modif]")
{
    M m{{1, "A"}, {2, "B"}};
    auto n = m.erase(static_cast<long>(1));
    CHECK(n == 1u);
    CHECK(!m.contains(1));
}

#endif // __cpp_lib_flat_set
