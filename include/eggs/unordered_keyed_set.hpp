// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_UNORDERED_KEYED_SET_HPP
#define EGGS_UNORDERED_KEYED_SET_HPP

#include <eggs/detail/hasher.hpp>
#include <eggs/detail/key_equal.hpp>
#include <eggs/detail/member_key_type.hpp>

#include <compare>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <unordered_set>
#include <utility>

namespace eggs
{
    ///////////////////////////////////////////////////////////////////////////
    //! \class unordered_keyed_set
    //!
    //! An unordered associative container that stores unique `Value` objects
    //! indexed by a member designated by the non-type template parameter
    //! `Key`, backed by `std::unordered_set`.
    //!
    //! Compared to `keyed_set`, `unordered_keyed_set` offers:
    //!   - O(1) average-case lookup, insertion, and erasure
    //!   - No ordering requirement on `key_type`
    //!
    //! At the cost of:
    //!   - No ordering guarantees on iteration
    //!   - No `lower_bound`, `upper_bound`, reverse iteration
    //!   - Requires `Hash` and `KeyEqual` in addition to (or instead of) a
    //!     comparator
    //!
    //! \tparam Value     The element type.
    //! \tparam Key       A pointer-to-member of `Value` whose pointee type
    //!                   becomes `key_type`.
    //! \tparam Hash      A hash function object for `key_type`. Defaults to
    //!                   `std::hash<key_type>`. If `Hash::is_transparent` is
    //!                   defined, heterogeneous lookup by arbitrary `K` is
    //!                   enabled (requires `KeyEqual::is_transparent` too).
    //! \tparam KeyEqual  An equality predicate on `key_type`. Defaults to
    //!                   `std::equal_to<key_type>`.
    template <typename Value, auto Key,
              typename Hash     = std::hash<typename keyed_set_detail::member_key_type<Value, Key>::type>,
              typename KeyEqual = std::equal_to<typename keyed_set_detail::member_key_type<Value, Key>::type>>
    class unordered_keyed_set
    {
        using _hasher    = keyed_set_detail::hasher<Value, Key, Hash>;
        using _key_equal = keyed_set_detail::key_equal<Value, Key, KeyEqual>;
        using _set_type  = std::unordered_set<Value, _hasher, _key_equal>;

    public:
        // ── Required type members ────────────────────────────────────────────

        using key_type        = typename _hasher::key_type;
        using value_type      = Value;
        using hasher          = Hash;
        using key_equal       = KeyEqual;
        using size_type       = typename _set_type::size_type;
        using difference_type = typename _set_type::difference_type;
        using reference       = typename _set_type::reference;
        using const_reference = typename _set_type::const_reference;
        using pointer         = typename _set_type::pointer;
        using const_pointer   = typename _set_type::const_pointer;

        using iterator        = typename _set_type::const_iterator;
        using const_iterator  = typename _set_type::const_iterator;
        using local_iterator        = typename _set_type::const_local_iterator;
        using const_local_iterator  = typename _set_type::const_local_iterator;

        using node_type          = typename _set_type::node_type;
        using insert_return_type = typename _set_type::insert_return_type;

        // ── Constructors ─────────────────────────────────────────────────────

        unordered_keyed_set()
            : unordered_keyed_set(0)
        {}

        explicit unordered_keyed_set(size_type bucket_count,
                                     hasher const& h = hasher{},
                                     key_equal const& eq = key_equal{})
            : set_(bucket_count, _hasher{h}, _key_equal{eq})
        {}

        template <typename InputIt>
        unordered_keyed_set(InputIt first, InputIt last,
                            size_type bucket_count = 0,
                            hasher const& h = hasher{},
                            key_equal const& eq = key_equal{})
            : set_(first, last, bucket_count, _hasher{h}, _key_equal{eq})
        {}

#ifdef __cpp_lib_ranges_to_container
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        unordered_keyed_set(std::from_range_t, R&& rg,
                            size_type bucket_count = 0,
                            hasher const& h = hasher{},
                            key_equal const& eq = key_equal{})
            : set_(std::ranges::begin(rg), std::ranges::end(rg),
                   bucket_count, _hasher{h}, _key_equal{eq})
        {}
#endif

        unordered_keyed_set(std::initializer_list<value_type> il,
                            size_type bucket_count = 0,
                            hasher const& h = hasher{},
                            key_equal const& eq = key_equal{})
            : set_(il, bucket_count, _hasher{h}, _key_equal{eq})
        {}

        unordered_keyed_set(unordered_keyed_set const&) = default;
        unordered_keyed_set(unordered_keyed_set&&) noexcept = default;

        ~unordered_keyed_set() = default;

        // ── Assignment ───────────────────────────────────────────────────────

        unordered_keyed_set& operator=(unordered_keyed_set const&) = default;
        unordered_keyed_set& operator=(unordered_keyed_set&&) noexcept = default;

        unordered_keyed_set& operator=(std::initializer_list<value_type> il)
        {
            set_ = il;
            return *this;
        }

        // ── Iterators ────────────────────────────────────────────────────────

        const_iterator begin()  const noexcept { return set_.begin();  }
        const_iterator end()    const noexcept { return set_.end();    }
        const_iterator cbegin() const noexcept { return set_.cbegin(); }
        const_iterator cend()   const noexcept { return set_.cend();   }

        // ── Capacity ─────────────────────────────────────────────────────────

        [[nodiscard]] bool empty()    const noexcept { return set_.empty();    }
        size_type          size()     const noexcept { return set_.size();     }
        size_type          max_size() const noexcept { return set_.max_size(); }

        // ── Modifiers ────────────────────────────────────────────────────────

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args)
        {
            return set_.emplace(std::forward<Args>(args)...);
        }

        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args)
        {
            return set_.emplace_hint(hint, std::forward<Args>(args)...);
        }

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

        void insert(std::initializer_list<value_type> il)
        {
            set_.insert(il);
        }

        insert_return_type insert(node_type&& nh)
        {
            return set_.insert(std::move(nh));
        }

        iterator insert(const_iterator hint, node_type&& nh)
        {
            return set_.insert(hint, std::move(nh));
        }

#ifdef __cpp_lib_ranges_to_container
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, value_type>
        void insert_range(R&& rg)
        {
            set_.insert(std::ranges::begin(rg), std::ranges::end(rg));
        }
#endif

        node_type extract(const_iterator q)
        {
            return set_.extract(q);
        }

        node_type extract(key_type const& k)
        {
            auto it = set_.find(k);
            if (it == set_.end()) return {};
            return set_.extract(it);
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        node_type extract(K&& k)
        {
            return set_.extract(std::forward<K>(k));
        }

        size_type erase(key_type const& k)
        {
            auto it = set_.find(k);
            if (it == set_.end()) return 0;
            set_.erase(it);
            return 1;
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        size_type erase(K&& k)
        {
            return set_.erase(std::forward<K>(k));
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

        void merge(unordered_keyed_set& source)
        {
            set_.merge(source.set_);
        }

        void merge(unordered_keyed_set&& source)
        {
            set_.merge(std::move(source.set_));
        }

        // ── Observers ────────────────────────────────────────────────────────

        hasher hash_function() const
        {
            return set_.hash_function().key_hash();
        }

        key_equal key_eq() const
        {
            return set_.key_eq().key_eq();
        }

        // ── Lookup ───────────────────────────────────────────────────────────

        const_iterator find(key_type const& k) const
        {
            return set_.find(k);
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        const_iterator find(K const& k) const
        {
            return set_.find(k);
        }

        size_type count(key_type const& k) const
        {
            return set_.find(k) != set_.end() ? 1 : 0;
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        size_type count(K const& k) const
        {
            return set_.count(k);
        }

        bool contains(key_type const& k) const
        {
            return set_.find(k) != set_.end();
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        bool contains(K const& k) const
        {
            return set_.contains(k);
        }

        std::pair<const_iterator, const_iterator>
        equal_range(key_type const& k) const
        {
            auto it = set_.find(k);
            if (it == set_.end()) return {set_.end(), set_.end()};
            auto next = std::next(it);
            return {it, next};
        }

        template <typename K>
            requires requires { typename Hash::is_transparent; } &&
                     requires { typename KeyEqual::is_transparent; }
        std::pair<const_iterator, const_iterator>
        equal_range(K const& k) const
        {
            return set_.equal_range(k);
        }

        // ── Bucket interface ─────────────────────────────────────────────────

        size_type bucket_count()     const noexcept { return set_.bucket_count();     }
        size_type max_bucket_count() const noexcept { return set_.max_bucket_count(); }

        size_type bucket_size(size_type n) const { return set_.bucket_size(n); }

        // Note: std::unordered_set::bucket() takes value_type; we reach
        // the bucket via find to stay key-type agnostic.
        size_type bucket(key_type const& k) const
        {
            return set_.bucket_count() > 0
                ? set_.hash_function()(k) % set_.bucket_count()
                : 0;
        }

        local_iterator begin(size_type n) const  { return set_.begin(n);  }
        local_iterator end(size_type n)   const  { return set_.end(n);    }
        local_iterator cbegin(size_type n) const { return set_.cbegin(n); }
        local_iterator cend(size_type n)   const { return set_.cend(n);   }

        // ── Hash policy ──────────────────────────────────────────────────────

        float load_factor()     const noexcept { return set_.load_factor();     }
        float max_load_factor() const noexcept { return set_.max_load_factor(); }

        void max_load_factor(float ml) { set_.max_load_factor(ml); }

        void rehash(size_type n)  { set_.rehash(n);  }
        void reserve(size_type n) { set_.reserve(n); }

        // ── Swap ─────────────────────────────────────────────────────────────

        void swap(unordered_keyed_set& other) noexcept
        {
            set_.swap(other.set_);
        }

        friend void swap(unordered_keyed_set& a, unordered_keyed_set& b) noexcept
        {
            a.swap(b);
        }

        // ── Comparison ───────────────────────────────────────────────────────

        friend bool operator==(unordered_keyed_set const& a,
                               unordered_keyed_set const& b)
        {
            return a.set_ == b.set_;
        }

    private:
        _set_type set_;
    };

    // ── Non-member erase_if ──────────────────────────────────────────────────

    template <typename Value, auto Key, typename Hash, typename KeyEqual,
              typename Predicate>
    typename unordered_keyed_set<Value, Key, Hash, KeyEqual>::size_type
    erase_if(unordered_keyed_set<Value, Key, Hash, KeyEqual>& c, Predicate pred)
    {
        auto original_size = c.size();
        for (auto i = c.begin(); i != c.end(); )
        {
            if (pred(*i)) { i = c.erase(i); }
            else          { ++i;            }
        }
        return original_size - c.size();
    }

} // namespace eggs

#endif // EGGS_UNORDERED_KEYED_SET_HPP
