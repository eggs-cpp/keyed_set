// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_DETAIL_KEY_EQUAL_HPP
#define EGGS_DETAIL_KEY_EQUAL_HPP

#include <eggs/detail/member_key_type.hpp>

#include <utility>

namespace eggs::keyed_set_detail
{
    ///////////////////////////////////////////////////////////////////////////
    //! \struct key_equal
    //!
    //! Projecting equality adaptor. Wraps a user-supplied `KeyEqual` and
    //! applies it to `v.*Key` rather than directly to `Value`.
    //!
    //! `is_transparent` is always defined so that `std::unordered_set`
    //! activates its heterogeneous lookup paths.
    //!
    //! The generic `K` overloads are only enabled when
    //! `KeyEqual::is_transparent` is defined.
    template <typename Value, auto Key, typename KeyEqual>
    struct key_equal : private KeyEqual  // EBO
    {
        using key_type = typename member_key_type<Value, Key>::type;

        // Always defined so that std::unordered_set activates its
        // heterogeneous lookup paths.
        using is_transparent = void;

        key_equal() = default;

        explicit key_equal(KeyEqual const& eq)
            : KeyEqual(eq)
        {}

        explicit key_equal(KeyEqual&& eq)
            : KeyEqual(std::move(eq))
        {}

        KeyEqual const& key_eq() const noexcept
        {
            return static_cast<KeyEqual const&>(*this);
        }

        // Value vs Value
        bool operator()(Value const& a, Value const& b) const
            noexcept(noexcept(key_eq()(a.*Key, b.*Key)))
        {
            return key_eq()(a.*Key, b.*Key);
        }

        // key_type vs Value — always available
        bool operator()(key_type const& k, Value const& b) const
            noexcept(noexcept(key_eq()(k, b.*Key)))
        {
            return key_eq()(k, b.*Key);
        }

        // Value vs key_type — always available
        bool operator()(Value const& a, key_type const& k) const
            noexcept(noexcept(key_eq()(a.*Key, k)))
        {
            return key_eq()(a.*Key, k);
        }

        // key_type vs key_type — forwarded directly, for internal use
        bool operator()(key_type const& a, key_type const& b) const
            noexcept(noexcept(key_eq()(a, b)))
        {
            return key_eq()(a, b);
        }

        // Generic K overloads — only when KeyEqual::is_transparent is defined
        template <typename K>
            requires requires { typename KeyEqual::is_transparent; }
        bool operator()(K const& k, Value const& b) const
            noexcept(noexcept(key_eq()(k, b.*Key)))
        {
            return key_eq()(k, b.*Key);
        }

        template <typename K>
            requires requires { typename KeyEqual::is_transparent; }
        bool operator()(Value const& a, K const& k) const
            noexcept(noexcept(key_eq()(a.*Key, k)))
        {
            return key_eq()(a.*Key, k);
        }
    };

} // namespace eggs::keyed_set_detail

#endif // EGGS_DETAIL_KEY_EQUAL_HPP
