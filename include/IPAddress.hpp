#pragma once

#include <cstring>
#include <array>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Socket
{
	struct IPAddress
	{
	public:
		static IPAddress Any(uint16_t portno) 		{ return IPAddress{INADDR_ANY, portno};}
		static IPAddress Loopback(uint16_t portno)  { return IPAddress{INADDR_LOOPBACK, portno};}
		static IPAddress Broadcast(uint16_t portno) { return IPAddress{INADDR_BROADCAST, portno};}
	
		IPAddress() {}
		IPAddress(const std::string& ipaddr, uint16_t port);
		IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t portno);
		IPAddress(const sockaddr_in_t& addr_in);		
		operator sockaddr_in_t() const;
		const uint8_t& operator[](size_t octet) const;
		uint8_t& operator[](size_t octet);
		bool operator==(const IPAddress& other) const;
		bool operator !=(const IPAddress& other) const;
		std::string addr_string() const;
		std::string port_string() const;
		std::string to_string() const;
		operator std::string() const { return this->to_string();}
	private:
		IPAddress(uint32_t ipaddr, uint16_t portno);
	};

	bool IPAddress::operator==(const IPAddress& other) const
	{
		return this->octets = other.octets && this->port == other.port;
	}

	bool IPAddress::operator !=(const IPAddress& other) const
	{
		return !(*this == other);
	}

	const uint8_t& IPAddress::operator[](size_t octet) const
	{
		return octets[octet];
	}

	uint8_t& IPAddress::operator[](size_t octet) const
	{
		return octets[octet];
	}
	

	IPAddress::IPAddress(uint32_t ipaddr, uint16_t portno)
	{
		*(uint32_t*)octets.data() = htonl(ipaddr);
		port = portno;
	}

	IPAddress::IPAddress(const char* ipaddr)
	{
		int ret = ::inet_pton(AF_INET, ipaddr.c_str(), (uint32_t*)octets.data());
		if (ret > 0) {
			port = portno;
		} else {
			throw InvalidAddressException(errno);
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

	operator IPAddress::sockaddr_in_t() const
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
	
}
