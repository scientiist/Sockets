#include "Sockets/IPAddress.hpp"
#include <Sockets/Exceptions.hpp>

namespace Socket
{
    bool IPAddress::operator==(const IPAddress& other) const
    {
        return this->octets == other.octets && this->port == other.port;
    }

    bool IPAddress::operator !=(const IPAddress& other) const
    {
        return !(*this == other);
    }

    const uint8_t& IPAddress::operator[](size_t octet) const
    {
        return octets[octet];
    }


    IPAddress::IPAddress(uint32_t ipaddr, uint16_t portno)
    {
        *(uint32_t*)octets.data() = htonl(ipaddr);
        port = portno;
    }

    IPAddress::IPAddress(const std::string& ipaddr, uint16_t portno)
    {
        int ret = ::inet_pton(AF_INET, ipaddr.c_str(), (uint32_t*)octets.data());
        if (ret > 0) {
            port = portno;
        } else {
            throw SocketException(strerror(errno));
        }
    }

    IPAddress::IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t portno)
    {
        octets[0] = a;
        octets[1] = b;
        octets[2] = c;
        octets[3] = d;
        port = portno;
    }

    IPAddress::IPAddress(const sockaddr_in_t& addr_in) {
        *(uint32_t*)octets.data() = addr_in.sin_addr.s_addr;
        port = ntohs(addr_in.sin_port);
    }

    IPAddress::operator sockaddr_in_t() const
    {
        sockaddr_in_t addr_in;
        std::memset(&addr_in, 0, sizeof(addr_in));
        addr_in.sin_family = AF_INET;
        addr_in.sin_addr.s_addr = *(uint32_t*)octets.data();
        addr_in.sin_port = htons(port);
        return addr_in;
    }

    std::string IPAddress::addr_string() const
    {
        return std::to_string(octets[0]) +
               '.' + std::to_string(octets[1]) +
               '.' + std::to_string(octets[2]) +
               '.' + std::to_string(octets[3]);
    }

    std::string IPAddress::port_string() const
    {
        return std::to_string(this->port);
    }

    std::string IPAddress::to_string() const
    {
        return this->addr_string() + ":" + this->port_string();
    }

    IPAddress IPAddress::Resolve(const std::string &uri, uint16_t port) {
        struct hostent *he = gethostbyname(uri.c_str());

        if (he == nullptr)
        {
            throw BadUriException("");
        }

        char *ip = inet_ntoa(*(struct in_addr*)he->h_addr_list[0]);
        return IPAddress(std::string(ip), port);
    }

}