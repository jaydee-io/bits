find_package(GTest REQUIRED)

add_executable(bits_tests)
target_link_libraries(bits_tests bits GTest::GTest GTest::Main)

target_sources(bits_tests PRIVATE
    test_insert.cpp
    test_extract.cpp
    test_classes.cpp
)
target_enable_warnings(bits_tests PRIVATE)

add_test(
    NAME bits_tests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bits_tests --gtest_color=yes
)