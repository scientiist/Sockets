#include <gtest/gtest.h>
#include <Sockets/IPAddress.hpp>
#include <Sockets/Exceptions.hpp>


TEST(IPAddressTest, IPAddress_Constructor_Default)
{
    Socket::IPAddress addr;
    ASSERT_TRUE(addr.addr_string() == "0.0.0.0");
    ASSERT_TRUE(addr.port == 0);
}

TEST(IPAddressTest, IPAddress_Constructor_String)
{
    Socket::IPAddress addr("127.0.0.1", 4444);
    ASSERT_TRUE(addr.addr_string() == "127.0.0.1");
    ASSERT_TRUE(addr.port == 4444);
}

TEST(IPAddressTest, IPAddress_Constructor_Octets)
{
    Socket::IPAddress addr(255, 0, 255, 0, 42069);
    ASSERT_TRUE(addr.octets[0]==255);
    ASSERT_TRUE(addr.octets[1]==0);
    ASSERT_TRUE(addr.octets[2]==255);
    ASSERT_TRUE(addr.octets[3]==0);
    ASSERT_TRUE(addr.port == 42069);
}

TEST(IPAddressTest, IPAddress_Constructor_Invalid_Address)
{
    ASSERT_THROW({
        Socket::IPAddress addr("127.0.0,", 4444);
    }, std::exception);
    ASSERT_THROW({
        Socket::IPAddress addr("www.google.com", 4444);
    }, std::exception);
}

TEST(IPAddressTest, IPAddress_Operator_Equality)
{
    Socket::IPAddress a("127.0.0.1", 42069);
    Socket::IPAddress b("127.0.0.1", 42069);
    Socket::IPAddress c("255.255.255.255", 666);

    ASSERT_TRUE(a == b);
    ASSERT_TRUE(a != c);
}

