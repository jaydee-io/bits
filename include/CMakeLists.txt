################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
set(LIB_NAME bits)

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_code_coverage(${LIB_NAME} INTERFACE)

install(DIRECTORY bits DESTINATION include)
install(TARGETS ${LIB_NAME} EXPORT BitsExport)

# Unit tests
set(UNITTESTS_NAME bits_tests)
add_unit_test(${UNITTESTS_NAME}
    bits/bits.test.insert_carray.cpp
    bits/bits.test.insert_cpparray.cpp
    bits/bits.test.extract_carray.cpp
    bits/bits.test.extract_cpparray.cpp
    bits/BitsDeserializer.test.cpp
    bits/BitsSerializer.test.cpp
)
target_enable_warnings(${UNITTESTS_NAME} PRIVATE)
target_compile_features(${UNITTESTS_NAME} PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(${UNITTESTS_NAME} PRIVATE bits)