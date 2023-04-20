#pragma once

#include <IPAddress.hpp>

#ifndef INPORT_ANY
#define INPORT_ANY
#

namespace Socket
{
	typedef struct sockaddr_in sockaddr_in_t;
	typedef struct sockaddr sockaddr_t;
	typedef std::vector<uint8_t> msg_t;
	class UdpSocket {
		private:
			int sock{-1};
			sockaddr_in_t self_addr{};
			socklen_t self_addr_len = sizeof(self_addr);
			sockaddr_in_t peer_addr{};
			socklen_t peer_addr_len = sizeof(peer_addr);
		public:
			UdpSocket();
			UdpSocket(int family, int flags);
			UdpSocket(int socket, addrinfo info, bool connected, bool bound);
			virtual ~UdpSocket();
	
			UdpSocket(const UdpSocket &socket) = default;
			void Open()
			{
				this->Close();
				sock = (int)::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
				if (this->IsClosed()) {
					throw SocketCreationException(strerror(errno));
				}
			}
			int Close()
			{
				if (!this->IsClosed())
				{
					#ifdef _WIN32
					int ret = ::shutdown(sock, SD_BOTH);
					#else
					int ret = ::shutdown(sock, SHUT_RDWR);
					#endif
	
					if (ret < 0)
					{
						throw SocketException(strerror(errno));
					}
					#ifdef _WIN32
					ret = ::closesocket(sock);
					#else
					ret = ::close(sock);
	
					if (ret < 0) {
						throw SocketCloseException(strerror(errno));
					}
					sock = -1;
				}
				return 0;
			}
			bool IsClosed() const { return sock < 0;}
			void Bind(const IPAddress& ipaddr)
			{
				self_addr = ipaddr;
				self_addr_len = sizeof(self_addr);
				int opt = 1;
				int ret = ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
				if (ret < 0)
				{
					throw SocketSetOptionException(strerror(errno));
				}
				ret = ::bind(sock, (sockaddr_t*)&self_addr, self_addr_len);
				if (ret < 0)
				{
					throw SocketBindingError(strerror(errno));
				}
				ret = ::getsockname(sock, (sockaddr_t*)&self_addr, &self_addr_len);
				if (ret < 0)
				{
					throw SocketGetNameError(strerror(errno));
				}
			}
			int Bind(uint16_t portno)
			{
				auto ipaddr = IPAddress::Any(portno);
				return this->Bind(ipaddr);
			}
			int BindAny()
			{
				return this->Bind(INPORT_ANY);
			}
			int BindAny(uint16_t& portno)
			{
				int ret = this->Bind(INPORT_ANY);
				if (ret < 0) {
					return ret;
				}
				portno = IPAddress{self_addr}.port;
				return 0;
			}
			int Connect(const IPAddress& ipaddr)
			{
				peer_addr = ipaddr;
				peer_addr_len = sizeof(peer_addr);
				int ret = ::connect(sock, (sockaddr_t*)&peer_addr, peer_addr_len);
				if (ret < 0) {
					throw SocketConnectException(strerror(errno));
				}
			}
			int Connect(uint16_t portno) 
			{
				auto ipaddr = IPAddress::Loopback(portno);
				return this->connect(ipaddr);
			}
			IPAddress GetSelfIP() const {return self_addr;}
			IPAddress GetPeerIP() const {return peer_addr;}
			template <typename T, typename = typename std::enable_if<sizeof(typename T::value_type) == sizeof(uint8_t)>::type>
			int Send(const T& message, const IPAddress& ipaddr) const
			{
				sockaddr_in_t addr_in = ipaddr;
				socklen_t addr_in_len = sizeof(addr_in);
				int ret = ::sendto(sock, (const char*)message.data(), message.size(), 0, (sockaddr_t*)&addr_in, addr_in_len);
	
				if (ret < 0)
				{
					throw SocketException(strerror(errno));
				}
				return ret;
			}
	
			template <typename T, typename = typename
				std::enable_if<sizeof(typename T::value_type) == sizeof(uint8_t)>::type>
			int Receive(T& message, IPAddress& ipaddr) const
			{
				sockaddr_in_t addr_in;
				socklen_t addr_in_len = sizeof(addr_in);
			typename T::value_type buffer[10 * 1024];
			int ret = ::recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (sockaddr_t*)&addr_in, &addr_in_len);
	
			if (ret < 0)
			{
				throw SocketReceiveException(strerror(errno));	
			}
			ipaddr = addr_in;
			message = { std::begin(buffer), std::begin(buffer)+ret};
			return ret;
		}
		int Broadcast(int opt) const
		{
			int ret = ::setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&opt, sizeof(opt));
			if (ret < 0) {
				throw SocketException(strerror(errno));
			}
			return 0;
		}
		int Interrupt() const
		{
			uint16_t portno = IPAddress{self_addr}.port;
			auto ipaddr -> IPAddress::Loopback(portno);
			return this->Send(msg_t{}. ipaddr);
		}
	};
}
