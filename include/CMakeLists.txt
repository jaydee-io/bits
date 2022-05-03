################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
set(LIB_NAME bits)

add_library(${LIB_NAME} INTERFACE
    bits/bits.h
    bits/BitsTraits.h

    # Serialization / Deserialization
    bits/BitsSerializer.h
    bits/BitsDeserializer.h

    # Flags / Enum / BitsField
    bits/Flags.h
    bits/Enum.h
    bits/BitsField.h

    # Implementation details
    bits/detail/BaseSerialization.h
    bits/detail/Serializer.h
    bits/detail/Deserializer.h
    bits/detail/BitsStream.h
    bits/detail/BitsStreamManipulation.h
    bits/detail/underlying_integral_type.h
    bits/detail/helper_macros.h
)

target_include_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_code_coverage(${LIB_NAME} INTERFACE)

install(DIRECTORY bits DESTINATION include FILES_MATCHING PATTERN "*.h")
install(TARGETS ${LIB_NAME} EXPORT BitsExport)

# Unit tests
set(UNITTESTS_NAME bits_tests)
add_unit_test(TARGET ${UNITTESTS_NAME}
    # All headers inclusion test
    bits/bits.test.cpp

    # Serialization / Deserialization
    bits/bits_insertion.test.cpp
    bits/bits_extraction.test.cpp
    bits/BitsSerializer.test.cpp
    bits/BitsDeserializer.test.cpp

    # Flags / Enum / BitsField
    bits/Flags.test.cpp
    bits/Enum.test.cpp
    bits/BitsField.test.cpp
)
target_enable_warnings(${UNITTESTS_NAME} PRIVATE)
target_compile_features(${UNITTESTS_NAME} PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(${UNITTESTS_NAME} PRIVATE bits)