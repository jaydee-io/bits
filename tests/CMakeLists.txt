################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
add_unit_test(bits_tests
    test_insert_carray.cpp
    test_insert_cpparray.cpp
    test_extract_carray.cpp
    test_extract_cpparray.cpp
    test_classes.cpp
    test_examples.cpp
)
target_enable_warnings(bits_tests PRIVATE)
target_compile_features(bits_tests PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(bits_tests PRIVATE bits)