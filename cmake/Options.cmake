################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
# User-settable options
option(BITS_BUILD_TESTS   "Build bits unit tests" ON)
option(BITS_CODE_COVERAGE "Build bits with code coverage" OFF)

# Internals options
set(BITS_CXX_STANDARD "cxx_std_20" CACHE INTERNAL "CXX Standard used to build bits")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
