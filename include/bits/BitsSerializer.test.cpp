////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>

#include <bits/BitsSerializer.h>

using ::testing::ElementsAre;

const size_t BUFFER_SIZE = 8;

//-----------------------------------------------------------------------------
//- Serializer / Deserializer common tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, BitsSkipped)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream.skip(4); ASSERT_EQ(stream.nbBitsStreamed(), 4);
    stream.skip(2); ASSERT_EQ(stream.nbBitsStreamed(), 6);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 14);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 22);
    stream.skip(8); ASSERT_EQ(stream.nbBitsStreamed(), 30);

    stream.reset();
    ASSERT_EQ(stream.nbBitsStreamed(), 0);
}

TEST(BitsSerializer, ChainedSkip)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_EQ(stream.nbBitsStreamed(), 0);
    stream
        .skip(4)
        .skip(2)
        .skip(8)
        .skip(8)
        .skip(8);
    
    ASSERT_EQ(stream.nbBitsStreamed(), 30);
}

//-----------------------------------------------------------------------------
//- BitsSerializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, NbBitsSerialized)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    ASSERT_EQ(serializer.nbBitsStreamed(), 0);
    serializer.insert<uint8_t>(0x0, 4); ASSERT_EQ(serializer.nbBitsStreamed(), 4);
    serializer.insert<uint8_t>(0x0, 2); ASSERT_EQ(serializer.nbBitsStreamed(), 6);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 14);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 22);
    serializer.insert<uint8_t>(0x0, 8); ASSERT_EQ(serializer.nbBitsStreamed(), 30);
}

TEST(BitsSerializer, ChainedInsert)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert(0x03, 4)
        .insert(0x01, 2)
        .insert(0x7F, 8)
        .insert(0xDC, 8)
        .insert(0x0D, 8)
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}

TEST(BitsSerializer, ChainedInsert_AutoSize)
{
    std::array<uint8_t, BUFFER_SIZE * 2> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert<uint8_t> (0xD8)
        .insert<uint8_t> (0xE9)
        .insert<uint16_t>(0xA5A5)
        .insert<uint32_t>(0xB6B6'B6B6)
        .insert<uint64_t>(0xC7C7'C7C7'C7C7'C7C7)
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7));
}

TEST(BitsSerializer, BitsManipulation_ChainedInsert)
{
    std::array<uint8_t, BUFFER_SIZE> buffer = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    bits::BitsSerializer serializer(buffer);

    serializer
        << bits::nbits(4) << 0x03    // 2 bits serialized
        << bits::nbits(2) << 0x01    // 4 bits serialized
        << uint8_t { 0x7F }          // 8 bits serialized
        << uint8_t { 0xDC }          // 8 bits serialized
        << uint8_t { 0x0D }          // 8 bits serialized
    ;
    
    ASSERT_THAT(buffer, ElementsAre(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00));
}
