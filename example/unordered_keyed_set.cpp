// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <eggs/unordered_keyed_set.hpp>

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include "types.hpp"


int main()
{
    // ── Basic usage ───────────────────────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Employee, &Employee::id> roster;

        roster.insert({101, "Alice", 95'000.0});
        roster.insert({102, "Bob",   80'000.0});
        roster.insert({103, "Carol", 110'000.0});

        // Heterogeneous lookup by int — no Employee construction required
        auto it = roster.find(102);
        assert(it != roster.end());
        assert(it->name == "Bob");

        // Duplicate keys are rejected
        auto [pos, inserted] = roster.insert({102, "NotBob", 0.0});
        assert(!inserted);
        assert(pos->name == "Bob");

        // Erase by key
        assert(roster.erase(101) == 1u);
        assert(!roster.contains(101));
        assert(roster.size() == 2u);

        std::cout << "Roster (unordered):\n";
        for (auto const& e : roster)
            std::cout << "  " << e.id << "  " << e.name << '\n';
    }

    // ── String key ───────────────────────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Product, &Product::sku> catalogue;

        catalogue.insert({"SKU-001", "Widget A", 9.99});
        catalogue.insert({"SKU-002", "Widget B", 14.99});
        catalogue.insert({"SKU-003", "Gadget C", 49.99});

        assert(catalogue.contains("SKU-002"));
        assert(!catalogue.contains("SKU-999"));

        auto it = catalogue.find("SKU-003");
        assert(it != catalogue.end());
        assert(it->price == 49.99);
    }

    // ── Transparent lookup ───────────────────────────────────────────────────
    {
        // A transparent hasher must define is_transparent and support
        // hashing all types we want to look up with (string, string_view).
        struct TransparentHash
        {
            using is_transparent = void;
            std::size_t operator()(std::string const& s)    const { return std::hash<std::string>{}(s); }
            std::size_t operator()(std::string_view sv)     const { return std::hash<std::string_view>{}(sv); }
        };

        using TransSet = eggs::unordered_keyed_set<
            Product,
            &Product::sku,
            TransparentHash,
            std::equal_to<>>;

        TransSet catalogue;
        catalogue.insert({"SKU-001", "Widget A", 9.99});
        catalogue.insert({"SKU-002", "Widget B", 14.99});

        // Lookup with string_view — zero allocation
        std::string_view sv = "SKU-001";
        auto it = catalogue.find(sv);
        assert(it != catalogue.end());
        assert(it->description == "Widget A");
    }

    // ── Node handle round-trip ────────────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Employee, &Employee::id> a, b;
        a.insert({1, "Alice", 95'000.0});
        a.insert({2, "Bob",   80'000.0});

        // Transfer element 1 from a to b without copying
        auto nh = a.extract(1);
        assert(!nh.empty());
        assert(a.size() == 1u);

        auto [pos, ok, node] = b.insert(std::move(nh));
        assert(ok);
        assert(b.contains(1));
    }

    // ── Bucket / load-factor interface ────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Employee, &Employee::id> m;
        m.reserve(200);   // pre-allocate for ~200 elements

        for (int i = 0; i < 100; ++i)
            m.insert({i, "e", 50'000.0});

        assert(m.size() == 100u);
        assert(m.load_factor() <= m.max_load_factor());

        m.rehash(256);
        assert(m.bucket_count() >= 256u);
    }

    // ── merge ─────────────────────────────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Employee, &Employee::id> dept_a, dept_b;
        dept_a.insert({1, "Alice", 95'000.0});
        dept_b.insert({2, "Bob",   80'000.0});
        dept_b.insert({3, "Carol", 90'000.0});

        dept_a.merge(dept_b);

        assert(dept_a.size() == 3u);
        assert(dept_b.empty());
    }

    // ── erase_if ──────────────────────────────────────────────────────────────
    {
        eggs::unordered_keyed_set<Employee, &Employee::id> m;
        for (int i = 1; i <= 6; ++i)
            m.insert({i, "e", static_cast<double>(i) * 10'000.0});

        auto n = eggs::erase_if(m,
            [](Employee const& e){ return e.salary < 40'000.0; });

        assert(n == 3u);
        assert(m.size() == 3u);
    }

    std::cout << "All assertions passed.\n";
}
