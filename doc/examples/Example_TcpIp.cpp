#include <bits/bits.h>

#include <iostream>
#include <iomanip>
#include <array>
#include <span>
#include <cstddef>

#include <pcap.h>

BITS_DECLARE_ENUM_WITH_TYPE(EtherType, uint16_t,
    PUP,         0x0200,
    IP,          0x0800,
    ARP,         0x0806,
    REVARP,      0x8035,
    VLAN,        0x8100,
    IPV6,        0x86dd,
    PAE,         0x888e,
    RSN_PREAUTH, 0x88c7,
    PTP,         0x88f7,
    LOOPBACK,    0x9000,
    IEEE802154,  0x0809
)

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
    ST,              7,               /* Stream protocol II */
    EGP,             8,               /* exterior gateway protocol */
    PIGP,            9,               /* private interior gateway */
    RCCMON,          10,              /* BBN RCC Monitoring */
    NVPII,           11,              /* network voice protocol*/
    PUP,             12,              /* pup */
    ARGUS,           13,              /* Argus */
    EMCON,           14,              /* EMCON */
    XNET,            15,              /* Cross Net Debugger */
    CHAOS,           16,              /* Chaos*/
    UDP,             17,              /* user datagram protocol */
    MUX,             18,              /* Multiplexing */
    MEAS,            19,              /* DCN Measurement Subsystems */
    HMP,             20,              /* Host Monitoring */
    PRM,             21,              /* Packet Radio Measurement */
    IDP,             22,              /* xns idp */
    TRUNK1,          23,              /* Trunk-1 */
    TRUNK2,          24,              /* Trunk-2 */
    LEAF1,           25,              /* Leaf-1 */
    LEAF2,           26,              /* Leaf-2 */
    RDP,             27,              /* Reliable Data */
    IRTP,            28,              /* Reliable Transaction */
    TP,              29,              /* tp-4 w/ class negotiation */
    BLT,             30,              /* Bulk Data Transfer */
    NSP,             31,              /* Network Services */
    INP,             32,              /* Merit Internodal */
    SEP,             33,              /* Sequential Exchange */
    THREE_PC,        34,              /* Third Party Connect */
    IDPR,            35,              /* InterDomain Policy Routing */
    XTP,             36,              /* XTP */
    DDP,             37,              /* Datagram Delivery */
    CMTP,            38,              /* Control Message Transport */
    TPXX,            39,              /* TP++ Transport */
    IL,              40,              /* IL transport protocol */
    IPV6,            41,              /* IP6 header */
    SDRP,            42,              /* Source Demand Routing */
    ROUTING,         43,              /* IP6 routing header */
    FRAGMENT,        44,              /* IP6 fragmentation header */
    IDRP,            45,              /* InterDomain Routing*/
    RSVP,            46,              /* resource reservation */
    GRE,             47,              /* General Routing Encap. */
    MHRP,            48,              /* Mobile Host Routing */
    BHA,             49,              /* BHA */
    ESP,             50,              /* IP6 Encap Sec. Payload */
    AH,              51,              /* IP6 Auth Header */
    INLSP,           52,              /* Integ. Net Layer Security */
    SWIPE,           53,              /* IP with encryption */
    NHRP,            54,              /* Next Hop Resolution */
    ICMPV6,          58,              /* ICMP6 */
    NONE,            59,              /* IP6 no next header */
    DSTOPTS,         60,              /* IP6 destination option */
    AHIP,            61,              /* any host internal protocol */
    CFTP,            62,              /* CFTP */
    HELLO,           63,              /* "hello" routing protocol */
    SATEXPAK,        64,              /* SATNET/Backroom EXPAK */
    KRYPTOLAN,       65,              /* Kryptolan */
    RVD,             66,              /* Remote Virtual Disk */
    IPPC,            67,              /* Pluribus Packet Core */
    ADFS,            68,              /* Any distributed FS */
    SATMON,          69,              /* Satnet Monitoring */
    VISA,            70,              /* VISA Protocol */
    IPCV,            71,              /* Packet Core Utility */
    CPNX,            72,              /* Comp. Prot. Net. Executive */
    CPHB,            73,              /* Comp. Prot. HeartBeat */
    WSN,             74,              /* Wang Span Network */
    PVP,             75,              /* Packet Video Protocol */
    BRSATMON,        76,              /* BackRoom SATNET Monitoring */
    ND,              77,              /* Sun net disk proto (temp.) */
    WBMON,           78,              /* WIDEBAND Monitoring */
    WBEXPAK,         79,              /* WIDEBAND EXPAK */
    EON,             80,              /* ISO cnlp */
    VMTP,            81,              /* VMTP */
    SVMTP,           82,              /* Secure VMTP */
    VINES,           83,              /* Banyon VINES */
    TTP,             84,              /* TTP */
    IGP,             85,              /* NSFNET-IGP */
    DGP,             86,              /* dissimilar gateway prot. */
    TCF,             87,              /* TCF */
    IGRP,            88,              /* Cisco/GXS IGRP */
    OSPFIGP,         89,              /* OSPFIGP */
    SRPC,            90,              /* Strite RPC protocol */
    LARP,            91,              /* Locus Address Resoloution */
    MTP,             92,              /* Multicast Transport */
    AX25,            93,              /* AX.25 Frames */
    IPEIP,           94,              /* IP encapsulated in IP */
    MICP,            95,              /* Mobile Int.ing control */
    SCCSP,           96,              /* Semaphore Comm. security */
    ETHERIP,         97,              /* Ethernet IP encapsulation */
    ENCAP,           98,              /* encapsulation header */
    APES,            99,              /* any private encr. scheme */
    GMTP,            100,             /* GMTP*/
    PIM,             103,             /* Protocol Independent Mcast */
    IPCOMP,          108,             /* payload compression (IPComp) */
    PGM,             113,             /* PGM */
    SCTP,            132,             /* SCTP */
    DIVERT,          254,             /* divert pseudo-protocol */
    RAW,             255              /* raw IP packet */
)

BITS_DECLARE_FLAGS_WITH_TYPE(Tcp, uint16_t,
    FIN,  0,
    SYN,  1,
    RST,  2,
    PUSH, 3,
    ACK,  4,
    URG,  5,
    ECE,  6,
    CWR,  7,
    NS,   8
)

struct EthernetHeader {
    std::array<uint8_t, 6> dest;
    std::array<uint8_t, 6> src;
    EtherType proto;
};

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

struct TcpHeader {
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint32_t sequenceNumber;
    uint32_t acknowledgementNumber;
    uint8_t dataOffset;
    FlagsTcp flags;
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
};

EthernetHeader extractEthernetHeader(std::span<const std::byte> buffer)
{
    bits::BitsDeserializer deserializer(buffer);
    EthernetHeader ethernetHeader;

    deserializer >> ethernetHeader.dest;
    deserializer >> ethernetHeader.src;
    deserializer >> ethernetHeader.proto;

    return ethernetHeader;
}

IpHeader extractIpHeader(std::span<const std::byte> buffer)
{
    bits::BitsDeserializer deserializer(buffer);
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
        >>                    ipHeader.source
        >>                    ipHeader.destination
    ;

    return ipHeader;
}

TcpHeader extractTcpHeader(std::span<const std::byte> buffer)
{
    bits::BitsDeserializer deserializer(buffer);
    TcpHeader tcpHeader;

    deserializer
        >>                    tcpHeader.sourcePort
        >>                    tcpHeader.destinationPort
        >>                    tcpHeader.sequenceNumber
        >>                    tcpHeader.acknowledgementNumber
        >> bits::nbits(4)  >> tcpHeader.dataOffset
        >> bits::skip(3)
        >> bits::nbits(9)  >> tcpHeader.flags
        >>                    tcpHeader.windowSize
        >>                    tcpHeader.checksum
        >>                    tcpHeader.urgentPointer
    ;

    return tcpHeader;
}

void printf(const char * format, std::string_view sv)
{
    printf(format, static_cast<int>(sv.size()), sv.data());
}

template<typename T, size_t N>
void printf(const char * format, char sep, const std::array<T, N> & array)
{
    for(auto i=0u; i<array.size(); i++)
    {
        if(i != 0u)
            printf("%c", sep);
        printf(format, array[i]);
    }
}

void printEthernetHeader(EthernetHeader & ethernetHeader)
{
    printf("ETH : ");
    printf("%02X", ':', ethernetHeader.src);
    printf(" -> ");
    printf("%02X", ':', ethernetHeader.dest);
    printf(" | %.*s", bits::to_string(ethernetHeader.proto));
    printf("\n");
}

void printIpHeader(IpHeader & ipHeader)
{
    printf("IP  : Version=%d | Header=%d bytes", ipHeader.version, ipHeader.ihl * 4);
    printf(" | ToS=%.*s", bits::to_string(ipHeader.tos));
    printf(" | PRIO=%d", ipHeader.precedence);
    printf(" | Length=%d", ipHeader.length);
    printf(" | ID=%d", ipHeader.identification);
    printf(" | Flags=%.*s", bits::to_string(ipHeader.flags));
    printf(" | Frag offset=%d", ipHeader.fragmentOffset);
    printf(" | TTL=%d", ipHeader.ttl);
    printf(" | Proto=%.*s", bits::to_string(ipHeader.protocol));
    printf(" | Check=0x%04X", ipHeader.checksum);
    printf(" | Src=");
    printf("%d", '.', ipHeader.source);
    printf(" | Dest=");
    printf("%d", '.', ipHeader.destination);
    printf("\n");
}

void printTcpHeader(TcpHeader & tcpHeader)
{
    printf("TCP : Dst=%d | Src=%d", tcpHeader.sourcePort, tcpHeader.destinationPort);
    printf(" | SEQ=%u", tcpHeader.sequenceNumber);
    printf(" | ACK=%u", tcpHeader.acknowledgementNumber);
    printf(" | Data Offset=%d bytes", tcpHeader.dataOffset * 4);
    printf(" | Flags=%.*s", bits::to_string(tcpHeader.flags));
    printf(" | Window=%d", tcpHeader.windowSize);
    printf(" | Checksum=0x%04X", tcpHeader.checksum);
    printf(" | URG=%d", tcpHeader.urgentPointer);
    printf("\n");
}

void printBuffer(std::span<const std::byte> buffer)
{
    for(size_t i=0; i<buffer.size(); i++)
    {
        printf("%08zX :", i);

        for(size_t j=0; j<8 and i<buffer.size(); j++, i++)
            printf(" %02hhX", std::to_integer<int>(buffer[i]));
        printf("    ");
        for(size_t j=0; j<8 and i<buffer.size(); j++, i++)
            printf(" %02hhX", std::to_integer<int>(buffer[i]));
        printf("    ");
        for(size_t j=0; j<8 and i<buffer.size(); j++, i++)
            printf(" %02hhX", std::to_integer<int>(buffer[i]));
        printf("    ");
        for(size_t j=0; j<8 and i<buffer.size(); j++, i++)
            printf(" %02hhX", std::to_integer<int>(buffer[i]));
        printf("\n");

        i--;
    }
}

void onPacket(uint8_t * /*args*/, const struct pcap_pkthdr * packet_header, const uint8_t * packet_body)
{
    auto packet = std::span(reinterpret_cast<const std::byte *>(packet_body), packet_header->caplen);
    auto ethernetHeader = extractEthernetHeader(packet);
    auto ipHeader = extractIpHeader(packet.subspan(sizeof(ethernetHeader)));

    printf("%ld.%06d - %6d bytes received", packet_header->ts.tv_sec, packet_header->ts.tv_usec, packet_header->caplen);

    if(ethernetHeader.proto != EtherType::IP or ipHeader.protocol != IpProtocol::TCP)
    {
        printf(" | ");
        printEthernetHeader(ethernetHeader);
        return;

    }

    auto tcpHeader = extractTcpHeader(packet.subspan(sizeof(ethernetHeader) + (ipHeader.ihl * 4)));

    printf("\n");
    printf("    "); printEthernetHeader(ethernetHeader);
    printf("    "); printIpHeader(ipHeader);
    printf("    "); printTcpHeader(tcpHeader);

    auto dataStart = sizeof(ethernetHeader) + (ipHeader.ihl * 4) + (tcpHeader.dataOffset * 4);
    printf("    DATA : %lu bytes\n", packet.size() - dataStart);
    if(dataStart < packet.size())
        printBuffer(packet.subspan(dataStart));
}

int main(int argc, char * argv[])
{
    char error_buffer[PCAP_ERRBUF_SIZE];
    int snapshot_len = 2048;
    int promiscuous = 0;
    int timeout = 1000;

    if(argc < 3) {
        std::cerr << "Usage : example_tcpip [-i|-f] [interface name|file name]\n";
        return 1;
    }

    pcap_t * handle = nullptr;

    using namespace std::literals::string_view_literals;
    if("-i"sv == argv[1]) {
        handle = pcap_open_live(argv[2], snapshot_len, promiscuous, timeout, error_buffer);
    }
    else if("-f"sv == argv[1]) {
        handle = pcap_open_offline(argv[2], error_buffer);
    }

    if(!handle)
    {
        std::cerr << "Error pcap_open_xxx(): " << error_buffer << '\n';
        return 1;
    }
    pcap_loop(handle, 0, onPacket, NULL);
    pcap_close(handle);

    return 0;
}