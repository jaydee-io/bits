#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>

#include <bits/bits.h>
#include <bits/BitsSerializer.h>
#include <bits/BitsDeserializer.h>

using ::testing::ElementsAre;

TEST(BitsDetail, BME680_read)
{
    std::array<uint8_t, 2> buffer = { 0x00, 0x20 };

    // ... Read 2 bytes from I2C bus, starting at address 0x2A, into the buffer ...

    enum class GasValid
    {
        INVALID = 0,
        VALID = 1,
    };

    auto gas_resistance_data  = bits::extract<uint16_t,  9,  0>(buffer);
    auto gas_is_valid_as_enum = bits::extract<GasValid, 10, 10>(buffer);
    auto gas_is_valid_as_bool = bits::extract<bool,     10, 10>(buffer);

    if(gas_is_valid_as_bool)
        std::cout << "Gas value = " << gas_resistance_data << std::endl;
    else
        std::cout << "Gas value NOT valid" << std::endl;

    if(gas_is_valid_as_enum == GasValid::VALID)
        std::cout << "Gas value = " << gas_resistance_data << std::endl;
    else
        std::cout << "Gas value NOT valid" << std::endl;
}

TEST(BitsDetail, BME680_write)
{
    std::array<uint8_t, 1> buffer = { 0x00 };

    // ... Read 1 byte from I2C bus, at address 0x74, into the buffer ...
    // ...
    // ... Update the temperature oversampling setting ...
    enum class Oversampling
    {
        NO_OVERSAMPLING = 0,
        OVERSAMPLING_X1 = 1,
        OVERSAMPLING_X2 = 2,
        OVERSAMPLING_X4 = 3,
        OVERSAMPLING_X8 = 4,
        OVERSAMPLING_X16 = 5,
    };
    enum Oversampling2
    {
        NO_OVERSAMPLING = 0,
        OVERSAMPLING_X1 = 1,
        OVERSAMPLING_X2 = 2,
        OVERSAMPLING_X4 = 3,
        OVERSAMPLING_X8 = 4,
        OVERSAMPLING_X16 = 5,
    };

    std::cout << "std::is_integral_v<Oversampling>  = " << std::is_integral_v<Oversampling> << std::endl;
    std::cout << "std::is_integral_v<Oversampling2> = " << std::is_integral_v<Oversampling2> << std::endl;
    std::cout << "std::is_integral_v<uint8_t>       = " << std::is_integral_v<uint8_t> << std::endl;

    bits::insert<Oversampling, 7, 5>(Oversampling::OVERSAMPLING_X4, buffer);

    // ... Write back the byte on I2C bus, at address 0x74 ...
}

TEST(BitsDetail, TimeMessage)
{
    enum class MessageType
    {
        REQUEST = 0x0,
        RESPONSE = 0x1,
        // ...
    };

    enum class MessageGroup
    {
        TIME = 0x0,
        // ...
    };

    enum class TimeServices
    {
        UPDATE = 0x0,
        RETRIEVE = 0x1,
        // ...
    };

    enum class TimeUpdateStatus
    {
        SUCCESS = 0,
        ERROR = 1,
    };

    using Second = uint32_t;
    using NanoSecond = uint32_t;


    std::array<uint8_t, 16> buffer = { };

    // ... Read incoming message into the buffer ...

    // Deserialize request
    bits::BitsDeserializer deserializer(buffer);
    MessageType type;
    MessageGroup group;
    TimeServices service;
    deserializer
        >> bits::nbits(2) >> type
        >> bits::nbits(2) >> group
        >> bits::nbits(4) >> service
        >> bits::skip(8); // Skip 'length' field

    // Check this is a time update request
    if(!(type == MessageType::REQUEST
    && group == MessageGroup::TIME
    && service == TimeServices::UPDATE))
        return;

    auto second = deserializer.extract<Second>(sizeof(Second));
    auto nanoSecond = deserializer.extract<NanoSecond>(sizeof(NanoSecond));

    std::cout << "Second = " << second << std::endl;
    std::cout << "Nano second = " << nanoSecond << std::endl;

    // Serialize response
    bits::BitsSerializer serializer(buffer);
    serializer
        << bits::nbits(2) << MessageType::RESPONSE
        << bits::nbits(2) << MessageGroup::TIME
        << bits::nbits(4) << TimeServices::UPDATE
        << bits::skip(8) // Skip 'length' field
        << bits::nbits(8) << TimeUpdateStatus::SUCCESS;

    // ... Write back response message from the buffer ...
}
