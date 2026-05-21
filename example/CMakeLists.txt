# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(_examples keyed_set unordered_keyed_set)
foreach(_example IN LISTS _examples)
    add_executable(_eggs_${_example}_example ${_example}.cpp types.hpp)
    target_link_libraries(_eggs_${_example}_example
        PRIVATE Eggs::KeyedSet)
    set_target_properties(_eggs_${_example}_example
        PROPERTIES
            FOLDER "example"
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            CXX_SCAN_FOR_MODULES OFF)
endforeach()
