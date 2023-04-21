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
		UdpSocket() {}
		UdpSocket(int family, int flags);
		UdpSocket(int socket, addrinfo info, bool connected, bool bound);
		virtual ~UdpSocket();
	
		UdpSocket(const UdpSocket &socket) = default;
		UdpSocket &operator=(const UdpSocket &socket) = delete;
		void Open();
		int Close();
		bool IsClosed() const { return sock < 0;}
		void Bind(const IPAddress& ipAddress);
		void Bind(uint16_t portno);
		void BindAny();
        void BindAny(uint16_t& portno);
        void Connect(const IPAddress& ipaddr);
        void Connect(uint16_t portno);
			
		[[nodiscard]] IPAddress GetSelfIP() const {return self_addr;}
		[[nodiscard]] IPAddress GetPeerIP() const {return peer_addr;}
        public:
        template <typename T>
		int Send(const T& message, const IPAddress& ipaddr) const;
        template <typename T>
		int Receive(T& message, IPAddress& ipaddr) const;
		int Broadcast(int opt) const;
		void Interrupt() const;
	private:
		int sock{-1};
		sockaddr_in_t self_addr{};
		socklen_t self_addr_len = sizeof(self_addr);
		sockaddr_in_t peer_addr{};
		socklen_t peer_addr_len = sizeof(peer_addr);
	};
}
