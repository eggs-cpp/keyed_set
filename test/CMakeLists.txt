# Copyright Agustin K-ballo Berge, Fusion Fenix 2026
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Disable C++20 module dependency scanning for all targets in this directory.
# Without this, MSVC under Ninja Multi-Config invokes /scanDependencies in a
# pre-pass that strips the standard library include paths, causing fatal errors
# when trying to include <functional>, <set>, and other standard headers.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# ── Vendored Catch2 v2 single-header ────────────────────────────────────────

add_library(_catch2 INTERFACE)
target_include_directories(_catch2 SYSTEM INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(_catch2 INTERFACE
    CATCH_CONFIG_FAST_COMPILE)

# ── Shared test main (compiled once, linked into every test executable) ──────

add_library(_eggs_test_main OBJECT catch.cpp)
target_link_libraries(_eggs_test_main
    PUBLIC
        Eggs::KeyedSet
        _catch2)
set_target_properties(_eggs_test_main
    PROPERTIES
        FOLDER "test"
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_SCAN_FOR_MODULES OFF)

# ── Helper: register one executable + CTest per source file in a suite ───────
#
# eggs_test_suite(<suite> <source>...)
#   <suite>    — subdirectory name (e.g. keyed_set, flat_keyed_set)
#   <source>.. — source filenames relative to <suite>/
#
# Creates targets named  _eggs_<suite>_test.<stem>
# and CTest entries named  <suite>.<stem>
#
function(eggs_test_suite suite)
    foreach(_src IN LISTS ARGN)
        get_filename_component(_name "${_src}" NAME_WLE)
        set(_target "_eggs_${suite}_test.${_name}")

        add_executable(${_target} "${suite}/${_src}")
        target_include_directories(${_target}
            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
        target_link_libraries(${_target}
            PRIVATE _eggs_test_main)
        set_target_properties(${_target}
            PROPERTIES
                FOLDER "test/${suite}"
                CXX_STANDARD 23
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF
                CXX_SCAN_FOR_MODULES OFF)

        add_test(
            NAME "${suite}.${_name}"
            COMMAND ${_target})
    endforeach()
endfunction()

# ── keyed_set suite ──────────────────────────────────────────────────────────

eggs_test_suite(keyed_set
    types.cpp
    cnstr.default.cpp
    cnstr.compare.cpp
    cnstr.range.cpp
    cnstr.from_range.cpp
    cnstr.init_list.cpp
    cnstr.copy.cpp
    cnstr.move.cpp
    assign.copy.cpp
    assign.move.cpp
    assign.init_list.cpp
    modif.emplace.cpp
    modif.insert.cpp
    modif.insert_range.cpp
    modif.extract.cpp
    modif.erase.cpp
    modif.clear.cpp
    modif.merge.cpp
    lookup.find.cpp
    lookup.count.cpp
    lookup.contains.cpp
    lookup.bounds.cpp
    lookup.equal_range.cpp
    observers.cpp
    compare.cpp
    allocator.cpp
    swap.cpp
    comparison.cpp
)

# ── unordered_keyed_set suite ────────────────────────────────────────────────

eggs_test_suite(unordered_keyed_set
    types.cpp
    cnstr.default.cpp
    cnstr.hash.cpp
    cnstr.range.cpp
    cnstr.from_range.cpp
    cnstr.init_list.cpp
    cnstr.copy.cpp
    cnstr.move.cpp
    assign.copy.cpp
    assign.init_list.cpp
    modif.emplace.cpp
    modif.insert.cpp
    modif.insert_range.cpp
    modif.extract.cpp
    modif.erase.cpp
    modif.erase_if.cpp
    modif.clear.cpp
    modif.merge.cpp
    lookup.find.cpp
    lookup.count.cpp
    lookup.contains.cpp
    lookup.equal_range.cpp
    observers.cpp
    bucket.cpp
    swap.cpp
    comparison.cpp
)
