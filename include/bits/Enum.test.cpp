////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <bits/Enum.h>

BITS_DECLARE_ENUM(TestWithoutType,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

BITS_DECLARE_ENUM_WITH_TYPE(TestType, uint8_t,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

namespace testNamespace {

BITS_DECLARE_ENUM(TestWithoutType,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

BITS_DECLARE_ENUM_WITH_TYPE(TestType, uint8_t,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

} // namespace testNamespace

TEST(Enum, ToString) {

    EXPECT_EQ(to_string(TestType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(TestType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(TestType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(TestType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(TestType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithoutType) {
    EXPECT_EQ(to_string(TestWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(TestWithoutType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(TestWithoutType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(TestWithoutType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(TestWithoutType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithNamespace) {

    EXPECT_EQ(to_string(testNamespace::TestType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(testNamespace::TestType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(testNamespace::TestType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(testNamespace::TestType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(testNamespace::TestType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithNamespace_WithoutType) {
    EXPECT_EQ(to_string(testNamespace::TestWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(testNamespace::TestWithoutType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(testNamespace::TestWithoutType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(testNamespace::TestWithoutType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(testNamespace::TestWithoutType::VAL_4), "VAL_4");
}
