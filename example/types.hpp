// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_EXAMPLE_TYPES_HPP
#define EGGS_EXAMPLE_TYPES_HPP

#include <string>

struct Employee
{
    int         id;
    std::string name;
    double      salary = 0.0;

    friend bool operator==(Employee const&, Employee const&) = default;
    friend auto operator<=>(Employee const&, Employee const&) = default;
};

struct Product
{
    std::string sku;
    std::string description;
    double      price = 0.0;

    friend bool operator==(Product const&, Product const&) = default;
    friend auto operator<=>(Product const&, Product const&) = default;
};

#endif // EGGS_EXAMPLE_TYPES_HPP
