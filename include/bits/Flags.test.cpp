////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bits/Flags.h>

#include <bits/bits_extraction.h>
#include <bits/bits_insertion.h>
#include <bits/BitsSerializer.h>
#include <bits/BitsDeserializer.h>
#include <array>
#include <cstddef>

using ::testing::ElementsAreArray;

template<typename T = std::byte, typename... Ts>
constexpr std::array<T, sizeof...(Ts)> make_array(Ts && ... args) noexcept
{
    return { T(std::forward<Ts>(args))... };
}

template<typename T = std::byte, typename... Ts>
constexpr std::array<const T, sizeof...(Ts)> make_const_array(Ts && ... args) noexcept
{
    return { T(std::forward<Ts>(args))... };
}

BITS_DECLARE_FLAGS(TestWithoutType,
    BIT_0, 0,
    BIT_1, 1,
    BIT_2, 2,
    BIT_3, 3,
    BIT_4, 4
)

BITS_DECLARE_FLAGS_WITH_TYPE(TestType, uint8_t,
    BIT_0, 0,
    BIT_1, 1,
    BIT_2, 2,
    BIT_3, 3,
    BIT_4, 4
)

BITS_DECLARE_FLAGS_WITH_NAMESPACE(testNamespace, TestWithoutType,
    NS_BIT_0, 0,
    NS_BIT_1, 1,
    NS_BIT_2, 2,
    NS_BIT_3, 3,
    NS_BIT_4, 4
)

BITS_DECLARE_FLAGS_WITH_TYPE_AND_NAMESPACE(testNamespace, TestType, uint8_t,
    NS_BIT_0, 0,
    NS_BIT_1, 1,
    NS_BIT_2, 2,
    NS_BIT_3, 3,
    NS_BIT_4, 4
)

TEST(Flags, Construction) {
    // Default construction
    FlagsTestType flags0;
    EXPECT_TRUE(!flags0);

    // Construction from enum type
    FlagsTestType flags1 = TestType::BIT_3;
    EXPECT_EQ(flags1, TestType::BIT_3);

    // Copy construction
    FlagsTestType flags2 = flags1;
    EXPECT_EQ(flags2, TestType::BIT_3);

    FlagsTestType flags3(5);
    EXPECT_EQ(flags3, TestType::BIT_0 | TestType::BIT_2);
}

TEST(Flags, Construction_WithNamespace) {
    // Default construction
    testNamespace::FlagsTestType flags0;
    EXPECT_TRUE(!flags0);

    // Construction from enum type
    testNamespace::FlagsTestType flags1 = testNamespace::TestType::NS_BIT_3;
    EXPECT_EQ(flags1, testNamespace::TestType::NS_BIT_3);

    // Copy construction
    testNamespace::FlagsTestType flags2 = flags1;
    EXPECT_EQ(flags2, testNamespace::TestType::NS_BIT_3);

    testNamespace::FlagsTestType flags3(5);
    EXPECT_EQ(flags3, testNamespace::TestType::NS_BIT_0 | testNamespace::TestType::NS_BIT_2);
}

TEST(Flags, Assignment) {
    FlagsTestType flags0 = TestType::BIT_3;

    // Direct assignment
    FlagsTestType flags1;
    flags1 = flags0;
    EXPECT_EQ(flags1, TestType::BIT_3);

    // Bitwise AND assignment
    FlagsTestType flags2 = TestType::BIT_3;
    flags2 &= flags0;
    EXPECT_EQ(flags2, TestType::BIT_3);

    // Bitwise OR assignment
    FlagsTestType flags3 = TestType::BIT_1;
    flags3 |= flags0;
    EXPECT_EQ(flags3, TestType::BIT_1 | TestType::BIT_3);

    // Bitwise XOR assignment
    FlagsTestType flags4 = TestType::BIT_3 | TestType::BIT_4;
    flags4 ^= flags0;
    EXPECT_EQ(flags4, TestType::BIT_4);
}

TEST(Flags, Assignment_WithNamespace) {
    testNamespace::FlagsTestType flags0 = testNamespace::TestType::NS_BIT_3;

    // Direct assignment
    testNamespace::FlagsTestType flags1;
    flags1 = flags0;
    EXPECT_EQ(flags1, testNamespace::TestType::NS_BIT_3);

    // Bitwise AND assignment
    testNamespace::FlagsTestType flags2 = testNamespace::TestType::NS_BIT_3;
    flags2 &= flags0;
    EXPECT_EQ(flags2, testNamespace::TestType::NS_BIT_3);

    // Bitwise OR assignment
    testNamespace::FlagsTestType flags3 = testNamespace::TestType::NS_BIT_1;
    flags3 |= flags0;
    EXPECT_EQ(flags3, testNamespace::TestType::NS_BIT_1 | testNamespace::TestType::NS_BIT_3);

    // Bitwise XOR assignment
    testNamespace::FlagsTestType flags4 = testNamespace::TestType::NS_BIT_3 | testNamespace::TestType::NS_BIT_4;
    flags4 ^= flags0;
    EXPECT_EQ(flags4, testNamespace::TestType::NS_BIT_4);
}

TEST(Flags, BitwiseOperators) {
    FlagsTestType flags0 = TestType::BIT_3;
    FlagsTestType flags1 = TestType::BIT_3;
    FlagsTestType flags2 = TestType::BIT_1;
    FlagsTestType flags3 = TestType::BIT_3 | TestType::BIT_4;
    FlagsTestType flags4 = TestType::BIT_1;
    FlagsTestType flags4Complementary = TestType::BIT_0 | TestType::BIT_2 | TestType::BIT_3 | TestType::BIT_4;

    // *** Flags/Flags bitwise operators ***
    EXPECT_EQ(flags0 & flags1, TestType::BIT_3);                            // Bitwise AND
    EXPECT_EQ(flags0 | flags2, TestType::BIT_1 | TestType::BIT_3);          // Bitwise OR
    EXPECT_EQ(flags0 ^ flags3, TestType::BIT_4);                            // Bitwise XOR

    // *** Enum/Flags bitwise operators ***
    EXPECT_EQ(TestType::BIT_3 & flags1, TestType::BIT_3);                   // Bitwise AND
    EXPECT_EQ(TestType::BIT_3 | flags2, TestType::BIT_1 | TestType::BIT_3); // Bitwise OR
    EXPECT_EQ(TestType::BIT_3 ^ flags3, TestType::BIT_4);                   // Bitwise XOR

    // *** Enum/Enum bitwise operators ***
    EXPECT_EQ(TestType::BIT_3 & TestType::BIT_3, TestType::BIT_3);                   // Bitwise AND
    EXPECT_EQ(TestType::BIT_3 | TestType::BIT_1, TestType::BIT_1 | TestType::BIT_3); // Bitwise OR
    EXPECT_EQ(TestType::BIT_3 ^ TestType::BIT_3, TestType());                        // Bitwise XOR

    // *** Unary bitwise operator ***
    EXPECT_EQ(~flags4,          flags4Complementary); // Bitwise NOT
    EXPECT_EQ(~TestType::BIT_1, flags4Complementary); // Bitwise NOT
}

TEST(Flags, BitwiseOperators_WithNamespace) {
    testNamespace::FlagsTestType flags0 = testNamespace::TestType::NS_BIT_3;
    testNamespace::FlagsTestType flags1 = testNamespace::TestType::NS_BIT_3;
    testNamespace::FlagsTestType flags2 = testNamespace::TestType::NS_BIT_1;
    testNamespace::FlagsTestType flags3 = testNamespace::TestType::NS_BIT_3 | testNamespace::TestType::NS_BIT_4;
    testNamespace::FlagsTestType flags4 = testNamespace::TestType::NS_BIT_1;
    testNamespace::FlagsTestType flags4Complementary = testNamespace::TestType::NS_BIT_0 | testNamespace::TestType::NS_BIT_2 | testNamespace::TestType::NS_BIT_3 | testNamespace::TestType::NS_BIT_4;

    // *** Flags/Flags bitwise operators ***
    EXPECT_EQ(flags0 & flags1, testNamespace::TestType::NS_BIT_3);                                      // Bitwise AND
    EXPECT_EQ(flags0 | flags2, testNamespace::TestType::NS_BIT_1 | testNamespace::TestType::NS_BIT_3);  // Bitwise OR
    EXPECT_EQ(flags0 ^ flags3, testNamespace::TestType::NS_BIT_4);                                      // Bitwise XOR

    // *** Enum/Flags bitwise operators ***
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 & flags1, testNamespace::TestType::NS_BIT_3);                                       // Bitwise AND
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 | flags2, testNamespace::TestType::NS_BIT_1 | testNamespace::TestType::NS_BIT_3);   // Bitwise OR
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 ^ flags3, testNamespace::TestType::NS_BIT_4);                                       // Bitwise XOR

    // *** Enum/Enum bitwise operators ***
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 & testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_3);                                     // Bitwise AND
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 | testNamespace::TestType::NS_BIT_1, testNamespace::TestType::NS_BIT_1 | testNamespace::TestType::NS_BIT_3); // Bitwise OR
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3 ^ testNamespace::TestType::NS_BIT_3, testNamespace::FlagsTestType());                                        // Bitwise XOR

    // *** Unary bitwise operator ***
    EXPECT_EQ(~flags4,                            flags4Complementary); // Bitwise NOT
    EXPECT_EQ(~testNamespace::TestType::NS_BIT_1, flags4Complementary); // Bitwise NOT
}

TEST(Flags, RelationnalOperators) {
    FlagsTestType flags = TestType::BIT_3;
    FlagsTestType flagsGreater = TestType::BIT_4;
    FlagsTestType flagsLesser = TestType::BIT_1;
    FlagsTestType flagsEqual = TestType::BIT_3;
    FlagsTestType flagsNotEqual = TestType::BIT_2;

    // *** Flags/Flags relationnal operators ***
    EXPECT_LT(flags, flagsGreater);     // Less than
    EXPECT_LE(flags, flagsGreater);     // Less than or equal to (less)
    EXPECT_LE(flags, flagsEqual);       // Less than or equal to (equal)
    EXPECT_GT(flags, flagsLesser);      // Greater than
    EXPECT_GE(flags, flagsLesser);      // Greater than or equal to (greater)
    EXPECT_GE(flags, flagsEqual);       // Greater than or equal to (equal)
    EXPECT_EQ(flags, flagsEqual);       // Equal to
    EXPECT_NE(flags, flagsNotEqual);    // Not equal to

    // *** Enum/Flags relationnal operators ***
    EXPECT_LT(TestType::BIT_3, flagsGreater);   // Less than
    EXPECT_LE(TestType::BIT_3, flagsGreater);   // Less than or equal to (less)
    EXPECT_LE(TestType::BIT_3, flagsEqual);     // Less than or equal to (equal)
    EXPECT_GT(TestType::BIT_3, flagsLesser);    // Greater than
    EXPECT_GE(TestType::BIT_3, flagsLesser);    // Greater than or equal to (greater)
    EXPECT_GE(TestType::BIT_3, flagsEqual);     // Greater than or equal to (equal)
    EXPECT_EQ(TestType::BIT_3, flagsEqual);     // Equal to
    EXPECT_NE(TestType::BIT_3, flagsNotEqual);  // Not equal to

    // *** Flags/Enum relationnal operators ***
    EXPECT_GT(flagsGreater  , TestType::BIT_3);  // Greater than
    EXPECT_GE(flagsGreater  , TestType::BIT_3);  // Greater than or equal to (greater)
    EXPECT_GE(flagsEqual    , TestType::BIT_3);  // Greater than or equal to (equal)
    EXPECT_LT(flagsLesser   , TestType::BIT_3);  // Less than
    EXPECT_LE(flagsLesser   , TestType::BIT_3);  // Less than or equal to (less)
    EXPECT_LE(flagsEqual    , TestType::BIT_3);  // Less than or equal to (equal)
    EXPECT_EQ(flagsEqual    , TestType::BIT_3);  // Equal to
    EXPECT_NE(flagsNotEqual , TestType::BIT_3);  // Not equal to

    // *** Enum/Enum relationnal operators (One Enum is implicitly converted to Flags) ***
    EXPECT_LT(TestType::BIT_3, TestType::BIT_4);    // Less than
    EXPECT_LE(TestType::BIT_3, TestType::BIT_4);    // Less than or equal to (less)
    EXPECT_LE(TestType::BIT_3, TestType::BIT_3);    // Less than or equal to (equal)
    EXPECT_GT(TestType::BIT_3, TestType::BIT_1);    // Greater than
    EXPECT_GE(TestType::BIT_3, TestType::BIT_1);    // Greater than or equal to (greater)
    EXPECT_GE(TestType::BIT_3, TestType::BIT_3);    // Greater than or equal to (equal)
    EXPECT_EQ(TestType::BIT_3, TestType::BIT_3);    // Equal to
    EXPECT_NE(TestType::BIT_3, TestType::BIT_2);    // Not equal to
}

TEST(Flags, RelationnalOperators_WithNamespace) {
    testNamespace::FlagsTestType flags = testNamespace::TestType::NS_BIT_3;
    testNamespace::FlagsTestType flagsGreater = testNamespace::TestType::NS_BIT_4;
    testNamespace::FlagsTestType flagsLesser = testNamespace::TestType::NS_BIT_1;
    testNamespace::FlagsTestType flagsEqual = testNamespace::TestType::NS_BIT_3;
    testNamespace::FlagsTestType flagsNotEqual = testNamespace::TestType::NS_BIT_2;

    // *** Flags/Flags relationnal operators ***
    EXPECT_LT(flags, flagsGreater);
    EXPECT_LE(flags, flagsGreater);
    EXPECT_LE(flags, flagsEqual);
    EXPECT_GT(flags, flagsLesser);
    EXPECT_GE(flags, flagsLesser);
    EXPECT_GE(flags, flagsEqual);
    EXPECT_EQ(flags, flagsEqual);
    EXPECT_NE(flags, flagsNotEqual);

    // *** Enum/Flags relationnal operators ***
    EXPECT_LT(testNamespace::TestType::NS_BIT_3, flagsGreater);
    EXPECT_LE(testNamespace::TestType::NS_BIT_3, flagsGreater);
    EXPECT_LE(testNamespace::TestType::NS_BIT_3, flagsEqual);
    EXPECT_GT(testNamespace::TestType::NS_BIT_3, flagsLesser);
    EXPECT_GE(testNamespace::TestType::NS_BIT_3, flagsLesser);
    EXPECT_GE(testNamespace::TestType::NS_BIT_3, flagsEqual);
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3, flagsEqual);
    EXPECT_NE(testNamespace::TestType::NS_BIT_3, flagsNotEqual);

    // *** Enum/Enum relationnal operators (One Enum is implicitly converted to Flags) ***
    EXPECT_LT(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_4);
    EXPECT_LE(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_4);
    EXPECT_LE(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_3);
    EXPECT_GT(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_1);
    EXPECT_GE(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_1);
    EXPECT_GE(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_3);
    EXPECT_EQ(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_3);
    EXPECT_NE(testNamespace::TestType::NS_BIT_3, testNamespace::TestType::NS_BIT_2);
}

TEST(Flags, Cast) {
    FlagsTestType flags0;
    EXPECT_TRUE(!flags0);
    EXPECT_EQ(uint8_t { 0 }, static_cast<uint8_t>(flags0));

    FlagsTestType flags1(3);
    EXPECT_TRUE(flags1);
    EXPECT_EQ(uint8_t { 3 }, static_cast<uint8_t>(flags1));
}

TEST(Flags, Cast_WithoutType) {
    FlagsTestWithoutType flags0;
    EXPECT_TRUE(!flags0);
    EXPECT_EQ(0, static_cast<int>(flags0));

    FlagsTestWithoutType flags1(3);
    EXPECT_TRUE(flags1);
    EXPECT_EQ(3, static_cast<int>(flags1));
}

TEST(Flags, Cast_WithNamespace) {
    testNamespace::FlagsTestType flags0;
    EXPECT_TRUE(!flags0);
    EXPECT_EQ(uint8_t { 0 }, static_cast<uint8_t>(flags0));

    testNamespace::FlagsTestType flags1(3);
    EXPECT_TRUE(flags1);
    EXPECT_EQ(uint8_t { 3 }, static_cast<uint8_t>(flags1));
}

TEST(Flags, Cast_WithNamespace_WithoutType) {
    testNamespace::FlagsTestWithoutType flags0;
    EXPECT_TRUE(!flags0);
    EXPECT_EQ(0, static_cast<int>(flags0));

    testNamespace::FlagsTestWithoutType flags1(3);
    EXPECT_TRUE(flags1);
    EXPECT_EQ(3, static_cast<int>(flags1));
}

TEST(Flags, SettingAndCheckingBits) {
    FlagsTestType flags;

    flags.set(TestType::BIT_1);
    EXPECT_EQ(TestType::BIT_1, flags);
    EXPECT_TRUE(flags.isSet(TestType::BIT_1));

    flags.toggle(TestType::BIT_3);
    EXPECT_EQ(TestType::BIT_1 | TestType::BIT_3, flags);
    EXPECT_TRUE(flags.isSet(TestType::BIT_3));

    flags.unset(TestType::BIT_3);
    EXPECT_EQ(TestType::BIT_1, flags);
    EXPECT_TRUE(flags.isNotSet(TestType::BIT_3));
}

TEST(Flags, SettingAndCheckingBits_WithNamespace) {
    testNamespace::FlagsTestType flags;

    flags.set(testNamespace::TestType::NS_BIT_1);
    EXPECT_EQ(testNamespace::TestType::NS_BIT_1, flags);
    EXPECT_TRUE(flags.isSet(testNamespace::TestType::NS_BIT_1));

    flags.toggle(testNamespace::TestType::NS_BIT_3);
    EXPECT_EQ(testNamespace::TestType::NS_BIT_1 | testNamespace::TestType::NS_BIT_3, flags);
    EXPECT_TRUE(flags.isSet(testNamespace::TestType::NS_BIT_3));

    flags.unset(testNamespace::TestType::NS_BIT_3);
    EXPECT_EQ(testNamespace::TestType::NS_BIT_1, flags);
    EXPECT_TRUE(flags.isNotSet(testNamespace::TestType::NS_BIT_3));
}

TEST(Flags, ToString) {
    // Empty flags
    FlagsTestType flags;
    EXPECT_EQ(bits::to_string(flags), "{}");

    // One flag
    flags |= TestType::BIT_2;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_2 }");

    // Multiple flags
    flags |= TestType::BIT_4;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_2 | BIT_4 }");

    // Flags are printed respecting order of declaration, not in order they have been set
    flags |= TestType::BIT_1;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_1 | BIT_2 | BIT_4 }");

    // All enum values
    flags |= TestType::BIT_0;
    flags |= TestType::BIT_3;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 }");
}

TEST(Flags, ToStringEnum) {
    EXPECT_EQ(bits::to_string(TestType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(bits::to_string(TestType::BIT_0), "BIT_0");
    EXPECT_EQ(bits::to_string(TestType::BIT_1), "BIT_1");
    EXPECT_EQ(bits::to_string(TestType::BIT_2), "BIT_2");
    EXPECT_EQ(bits::to_string(TestType::BIT_3), "BIT_3");
    EXPECT_EQ(bits::to_string(TestType::BIT_4), "BIT_4");
}

TEST(Flags, ToString_WithoutType) {
    // Empty flags
    FlagsTestWithoutType flags;
    EXPECT_EQ(bits::to_string(flags), "{}");

    // One flag
    flags |= TestWithoutType::BIT_2;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_2 }");

    // Multiple flags
    flags |= TestWithoutType::BIT_4;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_2 | BIT_4 }");

    // Flags are printed respecting order of declaration, not in order they have been set
    flags |= TestWithoutType::BIT_1;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_1 | BIT_2 | BIT_4 }");

    // All enum values
    flags |= TestWithoutType::BIT_0;
    flags |= TestWithoutType::BIT_3;
    EXPECT_EQ(bits::to_string(flags), "{ BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 }");
}

TEST(Flags, ToStringEnum_WithoutType) {
    EXPECT_EQ(bits::to_string(TestWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(bits::to_string(TestWithoutType::BIT_0), "BIT_0");
    EXPECT_EQ(bits::to_string(TestWithoutType::BIT_1), "BIT_1");
    EXPECT_EQ(bits::to_string(TestWithoutType::BIT_2), "BIT_2");
    EXPECT_EQ(bits::to_string(TestWithoutType::BIT_3), "BIT_3");
    EXPECT_EQ(bits::to_string(TestWithoutType::BIT_4), "BIT_4");
}

TEST(Flags, ToString_WithNamespace) {
    // Empty flags
    testNamespace::FlagsTestType flags;
    EXPECT_EQ(bits::to_string(flags), "{}");

    // One flag
    flags |= testNamespace::TestType::NS_BIT_2;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_2 }");

    // Multiple flags
    flags |= testNamespace::TestType::NS_BIT_4;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_2 | NS_BIT_4 }");

    // Flags are printed respecting order of declaration, not in order they have been set
    flags |= testNamespace::TestType::NS_BIT_1;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_1 | NS_BIT_2 | NS_BIT_4 }");

    // All enum values
    flags |= testNamespace::TestType::NS_BIT_0;
    flags |= testNamespace::TestType::NS_BIT_3;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_0 | NS_BIT_1 | NS_BIT_2 | NS_BIT_3 | NS_BIT_4 }");
}

TEST(Flags, ToStringEnum_WithNamespace) {
    EXPECT_EQ(bits::to_string(testNamespace::TestType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(bits::to_string(testNamespace::TestType::NS_BIT_0), "NS_BIT_0");
    EXPECT_EQ(bits::to_string(testNamespace::TestType::NS_BIT_1), "NS_BIT_1");
    EXPECT_EQ(bits::to_string(testNamespace::TestType::NS_BIT_2), "NS_BIT_2");
    EXPECT_EQ(bits::to_string(testNamespace::TestType::NS_BIT_3), "NS_BIT_3");
    EXPECT_EQ(bits::to_string(testNamespace::TestType::NS_BIT_4), "NS_BIT_4");
}

TEST(Flags, ToString_WithNamespace_WithoutType) {
    // Empty flags
    testNamespace::FlagsTestWithoutType flags;
    EXPECT_EQ(bits::to_string(flags), "{}");

    // One flag
    flags |= testNamespace::TestWithoutType::NS_BIT_2;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_2 }");

    // Multiple flags
    flags |= testNamespace::TestWithoutType::NS_BIT_4;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_2 | NS_BIT_4 }");

    // Flags are printed respecting order of declaration, not in order they have been set
    flags |= testNamespace::TestWithoutType::NS_BIT_1;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_1 | NS_BIT_2 | NS_BIT_4 }");

    // All enum values
    flags |= testNamespace::TestWithoutType::NS_BIT_0;
    flags |= testNamespace::TestWithoutType::NS_BIT_3;
    EXPECT_EQ(bits::to_string(flags), "{ NS_BIT_0 | NS_BIT_1 | NS_BIT_2 | NS_BIT_3 | NS_BIT_4 }");
}

TEST(Flags, ToStringEnum_WithNamespace_WithoutType) {
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType {}), "<invalid>"); // Empty enum
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType::NS_BIT_0), "NS_BIT_0");
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType::NS_BIT_1), "NS_BIT_1");
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType::NS_BIT_2), "NS_BIT_2");
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType::NS_BIT_3), "NS_BIT_3");
    EXPECT_EQ(bits::to_string(testNamespace::TestWithoutType::NS_BIT_4), "NS_BIT_4");
}

TEST(Flags, Size) {
    EXPECT_EQ(bits::size<TestType>(), 5);
    EXPECT_EQ(bits::size<TestWithoutType>(), 5);
    EXPECT_EQ(bits::size<testNamespace::TestType>(), 5);
    EXPECT_EQ(bits::size<testNamespace::TestWithoutType>(), 5);
}

TEST(Flags, Sizeof) {
    EXPECT_EQ(sizeof(FlagsTestType), sizeof(uint8_t));
    EXPECT_EQ(sizeof(FlagsTestWithoutType), sizeof(int));
    EXPECT_EQ(sizeof(testNamespace::FlagsTestType), sizeof(uint8_t));
    EXPECT_EQ(sizeof(testNamespace::FlagsTestWithoutType), sizeof(int));

    EXPECT_EQ(sizeof(TestType), sizeof(uint8_t));
    EXPECT_EQ(sizeof(TestWithoutType), sizeof(int));
    EXPECT_EQ(sizeof(testNamespace::TestType), sizeof(uint8_t));
    EXPECT_EQ(sizeof(testNamespace::TestWithoutType), sizeof(int));
}

TEST(Flags, Extraction) {
    const auto buffer = make_const_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);

    EXPECT_EQ(bits::extract<FlagsTestType>(buffer, 7,  4), TestType::BIT_0 | TestType::BIT_2);

    FlagsTestType flags;
    bits::extract(buffer, flags, 7,  4);
    EXPECT_EQ(flags, TestType::BIT_0 | TestType::BIT_2);
}

TEST(Flags, Insertion) {
    auto buffer = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    bits::insert(buffer, TestType::BIT_0 | TestType::BIT_2, 7,  4);
    EXPECT_THAT(buffer, ElementsAreArray(make_array(0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));

    FlagsTestType flags = TestType::BIT_0 | TestType::BIT_2;
    bits::insert(buffer, flags, 3,  0);
    EXPECT_THAT(buffer, ElementsAreArray(make_array(0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
}

TEST(Flags, Streaming) {
    auto buffer = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    // Serialization
    bits::BitsSerializer serializer(buffer);
    serializer << bits::nbits(4) << (TestType::BIT_0 | TestType::BIT_2);
    EXPECT_THAT(buffer, ElementsAreArray(make_array(0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));

    FlagsTestType flags1 = TestType::BIT_1 | TestType::BIT_3;
    serializer << flags1;
    EXPECT_THAT(buffer, ElementsAreArray(make_array(0x50, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));

    // Deserialization
    bits::BitsDeserializer deserializer(buffer);
    FlagsTestType flags2;
    deserializer >> bits::nbits(4) >> flags2;
    EXPECT_EQ(flags2, TestType::BIT_0 | TestType::BIT_2);

    FlagsTestType flags3;
    deserializer >> flags3;
    EXPECT_EQ(flags3, TestType::BIT_1 | TestType::BIT_3);
}