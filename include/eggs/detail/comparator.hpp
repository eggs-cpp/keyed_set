// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_DETAIL_COMPARATOR_HPP
#define EGGS_DETAIL_COMPARATOR_HPP

#include <eggs/detail/member_key_type.hpp>

#include <utility>

namespace eggs::keyed_set_detail
{
    ///////////////////////////////////////////////////////////////////////////
    //! \struct comparator
    //!
    //! Projecting comparator adaptor. Wraps a user-supplied `Compare`
    //! and applies it to `v.*Key` rather than directly to `Value`.
    //!
    //! `is_transparent` is always defined because `std::set` requires it
    //! to activate its heterogeneous tree operations (_M_find_tr etc.),
    //! which we always need since our `key_type` differs from `Value`.
    //!
    //! However, the generic `K` template overloads — which allow lookup
    //! by types other than `key_type` — are only enabled when
    //! `Compare::is_transparent` is defined. This matches the standard
    //! library convention: a non-transparent Compare (e.g. `std::less<int>`)
    //! is not expected to handle arbitrary `K`, so we do not expose those
    //! paths even though `is_transparent` itself is always present.
    template <typename Value, auto Key, typename Compare>
    struct comparator : private Compare  // EBO
    {
        using key_type = typename member_key_type<Value, Key>::type;

        // Always defined so that std::set activates its heterogeneous
        // tree operations. The actual type does not matter; void is the
        // conventional sentinel.
        using is_transparent = void;

        comparator() = default;

        explicit comparator(Compare const& c)
            : Compare(c)
        {}

        explicit comparator(Compare&& c)
            : Compare(std::move(c))
        {}

        Compare const& key_comp() const noexcept
        {
            return static_cast<Compare const&>(*this);
        }

        // Value vs Value
        constexpr bool operator()(Value const& a, Value const& b) const
            noexcept(noexcept(key_comp()(a.*Key, b.*Key)))
        { return key_comp()(a.*Key, b.*Key); }

        // key_type vs Value  — always available; projects b then compares
        constexpr bool operator()(key_type const& k, Value const& b) const
            noexcept(noexcept(key_comp()(k, b.*Key)))
        { return key_comp()(k, b.*Key); }

        // Value vs key_type  — always available
        constexpr bool operator()(Value const& a, key_type const& k) const
            noexcept(noexcept(key_comp()(a.*Key, k)))
        { return key_comp()(a.*Key, k); }

        // key_type vs key_type  — forwarded directly to Compare, so
        // std::set can compare extracted keys during rebalancing.
        constexpr bool operator()(key_type const& a, key_type const& b) const
            noexcept(noexcept(key_comp()(a, b)))
        { return key_comp()(a, b); }

        // Generic K overloads — only when Compare::is_transparent is
        // defined. A non-transparent Compare is not required to handle
        // arbitrary K, so these paths must not be exposed in that case.
        template <typename K>
            requires requires { typename Compare::is_transparent; }
        constexpr bool operator()(K const& k, Value const& b) const
            noexcept(noexcept(key_comp()(k, b.*Key)))
        { return key_comp()(k, b.*Key); }

        template <typename K>
            requires requires { typename Compare::is_transparent; }
        constexpr bool operator()(Value const& a, K const& k) const
            noexcept(noexcept(key_comp()(a.*Key, k)))
        { return key_comp()(a.*Key, k); }
    };

} // namespace eggs::keyed_set_detail

#endif // EGGS_DETAIL_COMPARATOR_HPP
