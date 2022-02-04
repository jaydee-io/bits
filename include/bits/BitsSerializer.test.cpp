////////////////////////////////////////////////////////////////////////////////
//                                    bits
//
// This file is distributed under the 3-clause Berkeley Software Distribution
// License. See LICENSE for details.
////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>
#include <cstddef>

#include <bits/BitsSerializer.h>

using ::testing::ElementsAreArray;

template<typename... Ts>
constexpr std::array<std::byte, sizeof...(Ts)> make_array(Ts && ... args) noexcept
{
    return { std::byte(std::forward<Ts>(args))... };
}

const size_t BUFFER_SIZE = 8;

//-----------------------------------------------------------------------------
//- Serializer / Deserializer common tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, BitsSkipped)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
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
    std::array<std::byte, BUFFER_SIZE> buffer = {};
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

TEST(BitsSerializer, OutOfRange)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer stream(buffer);

    ASSERT_THROW(stream.skip(128), std::out_of_range);
}

//-----------------------------------------------------------------------------
//- BitsSerializer specific tests
//-----------------------------------------------------------------------------
TEST(BitsSerializer, NbBitsSerialized)
{
    auto buffer = make_array(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
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
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert(uint8_t { 0x03 }, 4)
        .insert(uint8_t { 0x01 }, 2)
        .insert(uint8_t { 0x7F }, 8)
        .insert(uint8_t { 0xDC }, 8)
        .insert(uint8_t { 0x0D }, 8)
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x70, 0x34, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsSerializer, ChainedInsert_AutoSize)
{
    std::array<std::byte, BUFFER_SIZE * 2> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        .insert<uint8_t> (0xD8)
        .insert<uint8_t> (0xE9)
        .insert<uint16_t>(0xA5A5)
        .insert<uint32_t>(0xB6B6'B6B6)
        .insert<uint64_t>(0xC7C7'C7C7'C7C7'C7C7)
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7)));
}

TEST(BitsSerializer, ChainedInsert_Operator)
{
    std::array<std::byte, BUFFER_SIZE * 2> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        << uint8_t  { 0xD8 }
        << uint8_t  { 0xE9 }
        << uint16_t { 0xA5A5 }
        << uint32_t { 0xB6B6'B6B6 }
        << uint64_t { 0xC7C7'C7C7'C7C7'C7C7 }
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0xD8, 0xE9, 0xA5, 0xA5, 0xB6, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7)));
}

TEST(BitsSerializer, BitsManipulation_nbits)
{
    std::array<std::byte, BUFFER_SIZE> buffer = {};
    bits::BitsSerializer serializer(buffer);

    serializer
        << bits::nbits(4) << uint8_t { 0x03 }     // 4 bits serialized
        << bits::nbits(2) << uint8_t { 0x01 }     // 2 bits serialized
        << bits::nbits(4) << uint16_t { 0x07 }    // 4 bits serialized
        << bits::nbits(2) << uint32_t { 0x03 }    // 2 bits serialized
        << bits::nbits(4) << uint64_t { 0x0F }    // 4 bits serialized
    ;

    ASSERT_THAT(buffer, ElementsAreArray(make_array(0x35, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)));
}

TEST(BitsSerializer, BitsManipulation_SkipAndReset)
{
    auto buffer = make_array(0x35, 0xFF, 0x70, 0x35, 0xFF, 0x70, 0x35, 0xFF);
    bits::BitsSerializer serializer(buffer, 8);

    serializer << bits::skip(8);
    ASSERT_EQ(serializer.nbBitsStreamed(), 8);

    serializer << bits::reset();
    ASSERT_EQ(serializer.nbBitsStreamed(), 0);

    serializer << uint8_t(0xAA);
    ASSERT_EQ(buffer[1], std::byte(0xAA));
}
