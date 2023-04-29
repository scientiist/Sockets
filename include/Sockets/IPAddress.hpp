#pragma once

#include <cstring>
#include <array>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <Sockets/Exceptions.hpp>

namespace Socket
{

	typedef struct sockaddr_in sockaddr_in_t;
	typedef struct sockaddr sockaddr_t;
	struct IPAddress
	{
		std::array<uint8_t, 4> octets{};
		uint16_t port{};
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
		bool operator == (const IPAddress& other) const;
		bool operator != (const IPAddress& other) const;
		[[nodiscard]] std::string addr_string() const;
		[[nodiscard]] std::string port_string() const;
		[[nodiscard]] std::string to_string() const;
		operator std::string() const { return this->to_string();}
	private:
		IPAddress(uint32_t ipaddr, uint16_t portno);
	};


}
