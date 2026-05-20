// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_DETAIL_HASHER_HPP
#define EGGS_DETAIL_HASHER_HPP

#include <eggs/detail/member_key_type.hpp>

#include <utility>

namespace eggs::keyed_set_detail
{
    ///////////////////////////////////////////////////////////////////////////
    //! \struct hasher
    //!
    //! Projecting hash adaptor. Wraps a user-supplied `Hash` and applies it
    //! to `v.*Key` rather than directly to `Value`.
    //!
    //! `is_transparent` is always defined so that `std::unordered_set`
    //! activates its heterogeneous lookup paths, which we need because our
    //! `key_type` differs from `Value`.
    //!
    //! The generic `K` overload is only enabled when `Hash::is_transparent`
    //! is defined, matching the convention for transparent hashers.
    template <typename Value, auto Key, typename Hash>
    struct hasher : private Hash  // EBO
    {
        using key_type = typename member_key_type<Value, Key>::type;

        // Always defined so that std::unordered_set activates its
        // heterogeneous lookup paths.
        using is_transparent = void;

        hasher() = default;

        explicit hasher(Hash const& h)
            : Hash(h)
        {}

        explicit hasher(Hash&& h)
            : Hash(std::move(h))
        {}

        Hash const& key_hash() const noexcept
        {
            return static_cast<Hash const&>(*this);
        }

        // Hash a Value by projecting to key_type — always available
        std::size_t operator()(Value const& v) const
            noexcept(noexcept(key_hash()(v.*Key)))
        {
            return key_hash()(v.*Key);
        }

        // Hash a key_type directly — always available
        std::size_t operator()(key_type const& k) const
            noexcept(noexcept(key_hash()(k)))
        {
            return key_hash()(k);
        }

        // Generic K overload — only when Hash::is_transparent is defined
        template <typename K>
            requires requires { typename Hash::is_transparent; }
        std::size_t operator()(K const& k) const
            noexcept(noexcept(key_hash()(k)))
        {
            return key_hash()(k);
        }
    };

} // namespace eggs::keyed_set_detail

#endif // EGGS_DETAIL_HASHER_HPP
