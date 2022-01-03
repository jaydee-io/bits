////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <bits/BitsField.h>

TEST(BitsField, Construction) {
    // Default construction
    bits::BitsField<uint8_t, 9, 5> bitsfield0;
    EXPECT_EQ(bitsfield0, uint8_t {});

    // Copy construction from underlying type
    uint8_t val1 = 5;
    bits::BitsField<uint8_t, 9, 5> bitsfield1(val1);
    EXPECT_EQ(bitsfield1, val1);

    // Move construction from underlying type
    uint8_t val2 = 12;
    bits::BitsField<uint8_t, 9, 5> bitsfield2(std::move(val2));
    EXPECT_EQ(bitsfield2, val2);

    // Copy construction from BitsField type
    bits::BitsField<uint8_t, 9, 5> bitsfield3 = bitsfield1;
    EXPECT_EQ(bitsfield3, val1);

    // Move construction from BitsField type
    bits::BitsField<uint8_t, 9, 5> bitsfield4 = std::move(bitsfield2);
    EXPECT_EQ(bitsfield4, val2);
}

TEST(BitsField, Assignment) {
    uint8_t val0 = 15;
    bits::BitsField<uint8_t, 9, 5> bitsfield0(val0);

    // Copy assignment from underlying type
    bits::BitsField<uint8_t, 9, 5> bitsfield1;
    bitsfield1 = val0;
    EXPECT_EQ(bitsfield1, val0);

    // Move assignment from underlying type
    bits::BitsField<uint8_t, 9, 5> bitsfield2;
    bitsfield2 = std::move(val0);
    EXPECT_EQ(bitsfield2, val0);

    // Copy assignment from BitsField
    bits::BitsField<uint8_t, 9, 5> bitsfield3;
    bitsfield3 = bitsfield0;
    EXPECT_EQ(bitsfield3, val0);

    // Move assignment from BitsField
    bits::BitsField<uint8_t, 9, 5> bitsfield4;
    bitsfield4 = std::move(bitsfield0);
    EXPECT_EQ(bitsfield4, val0);
}

TEST(BitsField, RelationnalOperators) {
    bits::BitsField<uint8_t, 9, 5> bitsfield(3);
    bits::BitsField<uint8_t, 9, 5> bitsfieldGreater(4);
    bits::BitsField<uint8_t, 9, 5> bitsfieldLesser(1);
    bits::BitsField<uint8_t, 9, 5> bitsfieldEqual(3);
    bits::BitsField<uint8_t, 9, 5> bitsfieldNotEqual(2);

    // *** BitsField/BitsField relationnal operators ***
    EXPECT_LT(bitsfield, bitsfieldGreater);     // Less than
    EXPECT_LE(bitsfield, bitsfieldGreater);     // Less than or equal to (less)
    EXPECT_LE(bitsfield, bitsfieldEqual);       // Less than or equal to (equal)
    EXPECT_GT(bitsfield, bitsfieldLesser);      // Greater than
    EXPECT_GE(bitsfield, bitsfieldLesser);      // Greater than or equal to (greater)
    EXPECT_GE(bitsfield, bitsfieldEqual);       // Greater than or equal to (equal)
    EXPECT_EQ(bitsfield, bitsfieldEqual);       // Equal to
    EXPECT_NE(bitsfield, bitsfieldNotEqual);    // Not equal to

    // *** Underlying type/BitsField relationnal operators ***
    EXPECT_LT(3, bitsfieldGreater);   // Less than
    EXPECT_LE(3, bitsfieldGreater);   // Less than or equal to (less)
    EXPECT_LE(3, bitsfieldEqual);     // Less than or equal to (equal)
    EXPECT_GT(3, bitsfieldLesser);    // Greater than
    EXPECT_GE(3, bitsfieldLesser);    // Greater than or equal to (greater)
    EXPECT_GE(3, bitsfieldEqual);     // Greater than or equal to (equal)
    EXPECT_EQ(3, bitsfieldEqual);     // Equal to
    EXPECT_NE(3, bitsfieldNotEqual);  // Not equal to

    // *** BitsField/Underlying type relationnal operators ***
    EXPECT_GT(bitsfieldGreater  , 3);  // Greater than
    EXPECT_GE(bitsfieldGreater  , 3);  // Greater than or equal to (greater)
    EXPECT_GE(bitsfieldEqual    , 3);  // Greater than or equal to (equal)
    EXPECT_LT(bitsfieldLesser   , 3);  // Less than
    EXPECT_LE(bitsfieldLesser   , 3);  // Less than or equal to (less)
    EXPECT_LE(bitsfieldEqual    , 3);  // Less than or equal to (equal)
    EXPECT_EQ(bitsfieldEqual    , 3);  // Equal to
    EXPECT_NE(bitsfieldNotEqual , 3);  // Not equal to
}

TEST(BitsField, Cast) {
    bits::BitsField<uint8_t, 9, 5> bitsfield0;
    EXPECT_TRUE(!bitsfield0);

    bits::BitsField<uint8_t, 9, 5> bitsfield1(3);
    EXPECT_TRUE(bitsfield1);
}

TEST(BitsField, Access) {
    bits::BitsField<uint8_t, 9, 5> bitsfield0;
    bitsfield0++;
    EXPECT_EQ(uint8_t { 1 }, bitsfield0.get());

    bits::BitsField<uint8_t, 9, 5> bitsfield1(3);
    bitsfield1--;
    EXPECT_EQ(uint8_t { 2 }, bitsfield1.get());

    const bits::BitsField<uint8_t, 9, 5> bitsfield2(5);
    EXPECT_EQ(uint8_t { 5 }, bitsfield2.get());
}
