################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
cmake_minimum_required(VERSION 3.9)

project(bits CXX)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

include(cmake/utils.cmake)

# Main (header-only) library
add_subdirectory(include)

# Main library unit tests
enable_testing()
add_subdirectory(tests)

# Install CMake file for all exported targets
install(EXPORT BitsExport DESTINATION cmake/bits)
