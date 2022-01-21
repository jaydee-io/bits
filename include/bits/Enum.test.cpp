////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <bits/Enum.h>

BITS_DECLARE_ENUM(TestEnumWithoutType,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

BITS_DECLARE_ENUM(TestEnum8Values,
    VAL_1, 1,
    VAL_2, 2,
    VAL_3, 3,
    VAL_4, 4,
    VAL_5, 5,
    VAL_6, 6,
    VAL_7, 7,
    VAL_8, 8
)

BITS_DECLARE_ENUM_WITH_TYPE(TestEnumType, uint8_t,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

BITS_DECLARE_ENUM_WITH_NAMESPACE(testNamespace, TestEnumWithoutType,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

BITS_DECLARE_ENUM_WITH_TYPE_AND_NAMESPACE(testNamespace, TestEnumType, uint8_t,
    VAL_1, (1 * 10),
    VAL_2, (2 * 10),
    VAL_3, (3 * 10),
    VAL_4, (4 * 10)
)

TEST(Enum, ToString) {

    EXPECT_EQ(to_string(TestEnumType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(TestEnumType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(TestEnumType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(TestEnumType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(TestEnumType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithoutType) {
    EXPECT_EQ(to_string(TestEnumWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(TestEnumWithoutType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(TestEnumWithoutType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(TestEnumWithoutType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(TestEnumWithoutType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithNamespace) {

    EXPECT_EQ(to_string(testNamespace::TestEnumType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(testNamespace::TestEnumType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(testNamespace::TestEnumType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(testNamespace::TestEnumType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(testNamespace::TestEnumType::VAL_4), "VAL_4");
}

TEST(Enum, ToString_WithNamespace_WithoutType) {
    EXPECT_EQ(to_string(testNamespace::TestEnumWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(to_string(testNamespace::TestEnumWithoutType::VAL_1), "VAL_1");
    EXPECT_EQ(to_string(testNamespace::TestEnumWithoutType::VAL_2), "VAL_2");
    EXPECT_EQ(to_string(testNamespace::TestEnumWithoutType::VAL_3), "VAL_3");
    EXPECT_EQ(to_string(testNamespace::TestEnumWithoutType::VAL_4), "VAL_4");
}

TEST(Enum, Size) {
    EXPECT_EQ(bits::EnumTraits<TestEnum8Values>::size(), 8);
    EXPECT_EQ(bits::EnumTraits<TestEnumType>::size(), 4);
    EXPECT_EQ(bits::EnumTraits<TestEnumWithoutType>::size(), 4);
    EXPECT_EQ(bits::EnumTraits<testNamespace::TestEnumType>::size(), 4);
    EXPECT_EQ(bits::EnumTraits<testNamespace::TestEnumWithoutType>::size(), 4);
}

TEST(Enum, NotBitsEnum) {
    enum class TestEnumWrongEnum {
        VAL_1 = 1,
        VAL_2 = 2
    };

    EXPECT_FALSE(bits::is_enum_v<TestEnumWrongEnum>);

    // Trying to use bits::EnumTraits on non bits enum type raises a static_assert()
    // EXPECT_EQ(bits::EnumTraits<TestEnumWrongEnum>::size(), 2);
}