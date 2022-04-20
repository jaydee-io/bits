################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
cmake_minimum_required(VERSION 3.9)

project(bits CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(Options)
include(utils)
include(UnitTest)
include(CodeCoverage)

enable_unit_test(GOOGLETEST_VERSION "main")
enable_code_coverage()

# Main (header-only) library
add_subdirectory(include)

# Code example must compile
if(BITS_BUILD_TESTS)
    add_subdirectory(doc/examples)
endif()

# Install CMake file for all exported targets
install(EXPORT BitsExport DESTINATION cmake/bits)
