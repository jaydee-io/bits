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

# TCP/IP packet decoding example
set(EXAMPLE_NAME example_tcpip)
add_executable(${EXAMPLE_NAME} EXCLUDE_FROM_ALL
    Example_TcpIp.cpp
)
target_enable_warnings(${EXAMPLE_NAME} PRIVATE)
target_compile_features(${EXAMPLE_NAME} PRIVATE "cxx_std_20")
target_link_libraries(${EXAMPLE_NAME} PRIVATE bits pcap)