// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_FLAT_KEYED_SET_HPP
#define EGGS_FLAT_KEYED_SET_HPP

#include <version>

#ifdef __cpp_lib_flat_set

#include <eggs/detail/comparator.hpp>
#include <eggs/detail/member_key_type.hpp>

#include <flat_set>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace eggs
{
    ///////////////////////////////////////////////////////////////////////////
    //! \class flat_keyed_set
    //!
    //! An ordered associative container satisfying the C++23
    //! AssociativeContainer requirements that stores unique `Value` objects
    //! indexed by a member designated by the non-type template parameter
    //! `Key`, backed by a contiguous sorted sequence rather than a node-based
    //! tree.
    //!
    //! Compared to `keyed_set`, `flat_keyed_set` offers:
    //!   - O(1) amortised random-access iteration
    //!   - Better cache locality for lookup and iteration
    //!   - Smaller per-element overhead (no tree nodes)
    //!
    //! At the cost of:
    //!   - O(n) insertion and erasure (elements must be shifted)
    //!   - Iterator invalidation on any insertion or erasure
    //!   - No node handles (`extract`, `merge` are unavailable)
    //!
    //! \tparam Value      The element type.
    //! \tparam Key        A pointer-to-member of `Value` whose pointee type
    //!                    becomes `key_type`.
    //! \tparam Compare    A strict-weak-ordering binary predicate on
    //!                    `key_type`. Defaults to `std::less<key_type>`.
    //! \tparam KeyContainer  The underlying sorted container. Defaults to
    //!                    `std::vector<Value>`.
    template <typename Value, auto Key,
              typename Compare      = std::less<typename keyed_set_detail::member_key_type<Value, Key>::type>,
              typename KeyContainer = std::vector<Value>>
    class flat_keyed_set
    {
        using _comparator = keyed_set_detail::comparator<Value, Key, Compare>;
        using _set_type   = std::flat_set<Value, _comparator, KeyContainer>;

    public:
        // ── Required type members ────────────────────────────────────────────

        using key_type        = typename _comparator::key_type;
        using value_type      = Value;
        using key_compare     = Compare;
        using value_compare   = _comparator;
        using container_type  = KeyContainer;

        using size_type       = typename _set_type::size_type;
        using difference_type = typename _set_type::difference_type;
        using reference       = typename _set_type::reference;
        using const_reference = typename _set_type::const_reference;
        using iterator        = typename _set_type::iterator;
        using const_iterator  = typename _set_type::const_iterator;
        using reverse_iterator       = typename _set_type::reverse_iterator;
        using const_reverse_iterator = typename _set_type::const_reverse_iterator;

        // ── Constructors ─────────────────────────────────────────────────────

        flat_keyed_set() : flat_keyed_set(key_compare{}) {}

        explicit flat_keyed_set(key_compare const& c)
            : set_(_comparator{c})
        {}

        //! Construct from a pre-sorted, unique range — skips the sort step.
        flat_keyed_set(std::sorted_unique_t,
                       container_type cont,
                       key_compare const& c = key_compare{})
            : set_(std::sorted_unique, std::move(cont), _comparator{c})
        {}

        template <typename InputIt>
        flat_keyed_set(InputIt first, InputIt last,
                       key_compare const& c = key_compare{})
            : set_(first, last, _comparator{c})
        {}

        template <typename InputIt>
        flat_keyed_set(std::sorted_unique_t,
                       InputIt first, InputIt last,
                       key_compare const& c = key_compare{})
            : set_(std::sorted_unique, first, last, _comparator{c})
        {}

#ifdef __cpp_lib_ranges_to_container
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        flat_keyed_set(std::from_range_t, R&& rg,
                       key_compare const& c = key_compare{})
            : set_(std::ranges::begin(rg), std::ranges::end(rg), _comparator{c})
        {}
#endif

        flat_keyed_set(std::initializer_list<value_type> il,
                       key_compare const& c = key_compare{})
            : set_(il, _comparator{c})
        {}

        flat_keyed_set(std::sorted_unique_t,
                       std::initializer_list<value_type> il,
                       key_compare const& c = key_compare{})
            : set_(std::sorted_unique, il, _comparator{c})
        {}

        flat_keyed_set(flat_keyed_set const&) = default;
        flat_keyed_set(flat_keyed_set&&) noexcept = default;

        ~flat_keyed_set() = default;

        // ── Assignment ───────────────────────────────────────────────────────

        flat_keyed_set& operator=(flat_keyed_set const&) = default;
        flat_keyed_set& operator=(flat_keyed_set&&) noexcept = default;

        flat_keyed_set& operator=(std::initializer_list<value_type> il)
        {
            set_ = il;
            return *this;
        }

        // ── Underlying container access ──────────────────────────────────────

        //! Returns a const reference to the underlying sorted container.
        //! The elements are in sorted order by key.
        container_type const& keys() const noexcept
        {
            return set_.keys();   // std::flat_set::keys() returns const container_type&
        }

        //! Extracts the underlying container, leaving the flat_keyed_set in a
        //! valid but unspecified state.
        container_type extract_sequence() &&
        {
            return std::move(set_).extract();
        }

        //! Replaces the underlying container. `cont` must be sorted and contain
        //! no duplicate keys; behaviour is undefined otherwise.
        void replace_sequence(container_type&& cont)
        {
            set_.replace(std::move(cont));
        }

        // ── Iterators ────────────────────────────────────────────────────────

        //! \note Iterators are constant: elements must not be mutated through
        //! them as doing so could corrupt the ordering invariant.
        //! \note Any insertion or erasure invalidates all iterators.
        const_iterator  begin()  const noexcept { return set_.begin();  }
        const_iterator  end()    const noexcept { return set_.end();    }
        const_iterator  cbegin() const noexcept { return set_.cbegin(); }
        const_iterator  cend()   const noexcept { return set_.cend();   }

        const_reverse_iterator  rbegin()  const noexcept { return set_.rbegin();  }
        const_reverse_iterator  rend()    const noexcept { return set_.rend();    }
        const_reverse_iterator  crbegin() const noexcept { return set_.crbegin(); }
        const_reverse_iterator  crend()   const noexcept { return set_.crend();   }

        // ── Capacity ─────────────────────────────────────────────────────────

        [[nodiscard]] bool empty()    const noexcept { return set_.empty();    }
        size_type          size()     const noexcept { return set_.size();     }
        size_type          max_size() const noexcept { return set_.max_size(); }

        // ── Modifiers ────────────────────────────────────────────────────────

        //! \warning Invalidates all iterators.
        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args)
        {
            return set_.emplace(std::forward<Args>(args)...);
        }

        //! \warning Invalidates all iterators.
        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args)
        {
            return set_.emplace_hint(hint, std::forward<Args>(args)...);
        }

        //! \warning Invalidates all iterators.
        std::pair<iterator, bool> insert(value_type const& v)
        {
            return set_.insert(v);
        }

        std::pair<iterator, bool> insert(value_type&& v)
        {
            return set_.insert(std::move(v));
        }

        iterator insert(const_iterator hint, value_type const& v)
        {
            return set_.insert(hint, v);
        }

        iterator insert(const_iterator hint, value_type&& v)
        {
            return set_.insert(hint, std::move(v));
        }

        template <typename InputIt>
        void insert(InputIt first, InputIt last)
        {
            set_.insert(first, last);
        }

        template <typename InputIt>
        void insert(std::sorted_unique_t, InputIt first, InputIt last)
        {
            set_.insert(std::sorted_unique, first, last);
        }

        void insert(std::initializer_list<value_type> il)
        {
            set_.insert(il);
        }

        void insert(std::sorted_unique_t, std::initializer_list<value_type> il)
        {
            set_.insert(std::sorted_unique, il);
        }

#ifdef __cpp_lib_ranges_to_container
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        void insert_range(R&& rg)
        {
            set_.insert(std::ranges::begin(rg), std::ranges::end(rg));
        }
#endif

        //! \warning Invalidates all iterators.
        size_type erase(key_type const& k)
        {
            return set_.erase(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        size_type erase(K&& kx)
        {
            return set_.erase(std::forward<K>(kx));
        }

        iterator erase(const_iterator q)
        {
            return set_.erase(q);
        }

        iterator erase(const_iterator q1, const_iterator q2)
        {
            return set_.erase(q1, q2);
        }

        void clear() noexcept
        {
            set_.clear();
        }

        // ── Observers ────────────────────────────────────────────────────────

        key_compare key_comp() const
        {
            return set_.key_comp().key_comp();
        }

        value_compare value_comp() const
        {
            return set_.key_comp();
        }

        // ── Lookup ───────────────────────────────────────────────────────────

        const_iterator find(key_type const& k) const
        {
            return set_.find(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        const_iterator find(K const& k) const
        {
            return set_.find(k);
        }

        size_type count(key_type const& k) const
        {
            return set_.count(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        size_type count(K const& k) const
        {
            return set_.count(k);
        }

        bool contains(key_type const& k) const
        {
            return set_.contains(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        bool contains(K const& k) const
        {
            return set_.contains(k);
        }

        const_iterator lower_bound(key_type const& k) const
        {
            return set_.lower_bound(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        const_iterator lower_bound(K const& k) const
        {
            return set_.lower_bound(k);
        }

        const_iterator upper_bound(key_type const& k) const
        {
            return set_.upper_bound(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        const_iterator upper_bound(K const& k) const
        {
            return set_.upper_bound(k);
        }

        std::pair<const_iterator, const_iterator>
        equal_range(key_type const& k) const
        {
            return set_.equal_range(k);
        }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        std::pair<const_iterator, const_iterator>
        equal_range(K const& k) const
        {
            return set_.equal_range(k);
        }

        // ── Swap ─────────────────────────────────────────────────────────────

        void swap(flat_keyed_set& other) noexcept
        {
            set_.swap(other.set_);
        }

        // ── Comparison ───────────────────────────────────────────────────────

        friend bool operator==(flat_keyed_set const& a, flat_keyed_set const& b)
        {
            if (a.size() != b.size()) return false;
            return std::equal(a.begin(), a.end(), b.begin(),
                [](value_type const& x, value_type const& y)
                { return x.*Key == y.*Key; });
        }

        friend auto operator<=>(flat_keyed_set const& a, flat_keyed_set const& b)
        {
            return std::lexicographical_compare_three_way(
                a.begin(), a.end(),
                b.begin(), b.end(),
                [](value_type const& x, value_type const& y)
                { return x.*Key <=> y.*Key; });
        }

        friend void swap(flat_keyed_set& a, flat_keyed_set& b) noexcept
        {
            a.swap(b);
        }

    private:
        _set_type set_;
    };

} // namespace eggs

#endif // __cpp_lib_flat_set

#endif // EGGS_FLAT_KEYED_SET_HPP
