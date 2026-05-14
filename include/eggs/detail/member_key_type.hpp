// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_DETAIL_MEMBER_KEY_TYPE_HPP
#define EGGS_DETAIL_MEMBER_KEY_TYPE_HPP

#include <type_traits>

namespace eggs::keyed_set_detail
{
    ///////////////////////////////////////////////////////////////////////////
    //! \struct member_key_type
    //!
    //! Extracts the value type of a pointer-to-member via partial
    //! specialisation. This indirection is required because the NTTP
    //! syntax `auto Value::*Key` is not yet supported by all C++20/23
    //! compilers (notably GCC < 14); accepting `auto Key` and
    //! specialising on `T Value::*` is universally supported.
    template <typename Value, auto Key>
    struct member_key_type;

    template <typename Value, typename T, T Value::*Key>
    struct member_key_type<Value, Key>
    {
        using type = std::remove_cvref_t<T>;
    };

} // namespace eggs::keyed_set_detail

#endif // EGGS_DETAIL_MEMBER_KEY_TYPE_HPP
