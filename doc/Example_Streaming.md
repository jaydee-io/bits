`bits` - Bits streaming
=================================

## Example : _Message (de)serialization_
Another use case where `bits` could be very usefull, is for message serializing / deserialzing. This allows to extract meaningfull information and raise up abstraction level from low-level messages.

The following code shows an example of a server receiving and decoding a time update request and then encode and send back a response. Says that messages are formated this way:
```c++
// Field size in bits:   |   2   |   2   |    4    |    8   |   32   |     32     |
//                       ---------------------------------------------------------
// Time update request:  | Type  | Group | Service | Length | Second | Nanosecond |
//                       ---------------------------------------------------------

// Field size in bits:   |   2   |   2   |    4    |    8   |    8   |
//                       ---------------------------------------------
// Time update response: | Type  | Group | Service | Length | Status |
//                       ---------------------------------------------
```

First we define some meaningfull types for the messages.
```c++
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
````

Then we can receive the request, decode it, encode a response and send it back:
```c++
#include <bits/BitsSerializer.h>
#include <bits/BitsDeserializer.h>

std::array<std::byte, 16> buffer = { };

// ... Read incoming message into the buffer ...

// Deserialize request
bits::BitsDeserializer deserializer(buffer, buffer.size() * CHAR_BIT);
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
bits::BitsSerializer serializer(buffer, buffer.size() * CHAR_BIT);
serializer
    << bits::nbits(2) << MessageType::RESPONSE
    << bits::nbits(2) << MessageGroup::TIME
    << bits::nbits(4) << TimeServices::UPDATE
    << bits::skip(8) // Skip 'length' field
    << bits::nbits(8) << TimeUpdateStatus::SUCCESS;

// ... Write back response message from the buffer ...
```

## Example : _TCP/IP Packet deserialization_
`bits` stream could also be used to build a network stack and decode network packets. See full example in [Example_TcpIp.cpp](examples/Example_TcpIp.cpp).

First, we could use `bits` to declare IP flags and enums:
```c++
BITS_DECLARE_FLAGS_WITH_TYPE(TypeOfService, uint8_t,
    LOW_DELAY,        0,
    HIGH_THROUGHPUT,  1,
    HIGH_RELIABILITY, 2
)

BITS_DECLARE_FLAGS_WITH_TYPE(Fragments, uint8_t,
    DONT_FRAGMENT,  1,
    MORE_FRAGMENTS, 2
)

BITS_DECLARE_ENUM_WITH_TYPE(IpProtocol, uint8_t,
    HOPOPTS,         0,               /* IP6 hop-by-hop options */
    ICMP,            1,               /* control message protocol */
    IGMP,            2,               /* group mgmt protocol */
    GGP,             3,               /* gateway^2 (deprecated) */
    IPV4,            4,               /* IPv4 encapsulation */
    TCP,             6,               /* tcp */
    ...
)
```
Then, we could declare a struct for the IP header and use a `BitsDeserializer` to decode the packet:
```c++
struct IpHeader {
    uint8_t version;
    uint8_t ihl; // internet header length
    FlagsTypeOfService tos; // type of services
    uint8_t precedence; // priority
    uint16_t length;
    uint16_t identification;
    FlagsFragments flags;
    uint16_t fragmentOffset;
    uint8_t ttl;
    IpProtocol protocol;
    uint16_t checksum;
    std::array<uint8_t, 4> source;
    std::array<uint8_t, 4> destination;
};

IpHeader extractIpHeader(std::span<const std::byte> buffer)
{
    bits::BitsDeserializer deserializer(buffer.data(), buffer.size() * CHAR_BIT);
    IpHeader ipHeader;

    deserializer
        >> bits::nbits(4)  >> ipHeader.version
        >> bits::nbits(4)  >> ipHeader.ihl
        >> bits::nbits(5)  >> ipHeader.tos
        >> bits::nbits(3)  >> ipHeader.precedence
        >>                    ipHeader.length
        >>                    ipHeader.identification
        >> bits::nbits(3)  >> ipHeader.flags
        >> bits::nbits(13) >> ipHeader.fragmentOffset
        >>                    ipHeader.ttl
        >>                    ipHeader.protocol
        >>                    ipHeader.checksum
    ;

    // For now, bits doesn't support array (de)serialization, but it's coming soon :-)
    for(auto i=0ul; i<sizeof(ipHeader.source); i++)
        deserializer >> ipHeader.source[i];
    for(auto i=0ul; i<sizeof(ipHeader.destination); i++)
        deserializer >> ipHeader.destination[i];

    return ipHeader;
}
```

Finaly, we could display the IP header:
```c++
void printf(const char * format, std::string_view sv)
{
    printf(format, static_cast<int>(sv.size()), sv.data());
}

void printIpHeader(IpHeader & ipHeader)
{
    printf("IP  : Version=%d | Header=%d bytes", ipHeader.version, ipHeader.ihl * 4);
    printf(" | ToS=%.*s", bits::to_string(ipHeader.tos));
    // ...
    printf(" | Flags=%.*s", bits::to_string(ipHeader.flags));
    // ...
    printf(" | Proto=%.*s", bits::to_string(ipHeader.protocol));
    // ...
}
```