#pragma once

#include <IPAddress.hpp>

#ifndef INPORT_ANY
#define INPORT_ANY 0
#endif

namespace Socket
{
	typedef struct sockaddr_in sockaddr_in_t;
	typedef struct sockaddr sockaddr_t;
	typedef std::vector<uint8_t> msg_t;
	class UdpSocket {
	public:
		UdpSocket();
		UdpSocket(int family, int flags);
		UdpSocket(int socket, addrinfo info, bool connected, bool bound);
		virtual ~UdpSocket();
	
		UdpSocket(const UdpSocket &socket) = default;
		UdpSocket &operator=(const TcpSocket &socket) = delete;
		void Open();
		int Close();
		bool IsClosed() const { return sock < 0;}
		void Bind(const IPAddress& ipaddr);
		int Bind(uint16_t portno);
		int BindAny();
		int BindAny(uint16_t& portno);
		int Connect(const IPAddress& ipaddr);
		int Connect(uint16_t portno);
			
		IPAddress GetSelfIP() const {return self_addr;}
		IPAddress GetPeerIP() const {return peer_addr;}
		template <typename T, typename = typename std::enable_if<sizeof(typename T::value_type) == sizeof(uint8_t)>::type>
		int Send(const T& message, const IPAddress& ipaddr) const;	
		template <typename T, typename = typename
				std::enable_if<sizeof(typename T::value_type) == sizeof(uint8_t)>::type>
		int Receive(T& message, IPAddress& ipaddr) const;
		int Broadcast(int opt) const;
		int Interrupt() const;
	private:
		int sock{-1};
		sockaddr_in_t self_addr{};
		socklen_t self_addr_len = sizeof(self_addr);
		sockaddr_in_t peer_addr{};
		socklen_t peer_addr_len = sizeof(peer_addr);
	};
}
