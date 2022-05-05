################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
set(EXAMPLE_NAME example_tests)
add_executable(${EXAMPLE_NAME}
    Examples.test.cpp
)
target_enable_warnings(${EXAMPLE_NAME} PRIVATE)
target_compile_features(${EXAMPLE_NAME} PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(${EXAMPLE_NAME} PRIVATE bits)

# TCP/IP packet decoding example
set(EXAMPLE_NAME example_tcpip)
add_executable(${EXAMPLE_NAME}
    Example_TcpIp.cpp
)
target_enable_warnings(${EXAMPLE_NAME} PRIVATE)
target_compile_features(${EXAMPLE_NAME} PRIVATE ${BITS_CXX_STANDARD})
target_link_libraries(${EXAMPLE_NAME} PRIVATE bits)
if(NOT WIN32)
    target_link_libraries(${EXAMPLE_NAME} PRIVATE pcap)
endif()