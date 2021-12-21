################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
set(UNITTESTS_NAME example_tests)
add_unit_test(TARGET ${UNITTESTS_NAME} NO_COVERAGE
    Examples.test.cpp
)
target_enable_warnings(${UNITTESTS_NAME} PRIVATE)
target_compile_features(${UNITTESTS_NAME} PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(${UNITTESTS_NAME} PRIVATE bits)