#include "UdpSocket.hpp"

namespace Socket
{
	void UdpSocket::Open()
	{
		this->Close();
		sock = (int)::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (this->IsClosed()) {
			throw SocketCreationException(strerror(errno));
		}
	}

	int UdpSocket::Close()
	{
		if (!this->IsClosed()){
		#ifdef _WIN32
			int ret = ::shutdown(sock, SD_BOTH);
		#else
			int ret = ::shutdown(sock, SHUT_RDWR);
		#endif
	
			if (ret < 0) {
				throw SocketException(strerror(errno));
			}
		#ifdef _WIN32
			ret = ::closesocket(sock);
		#else
			ret = ::close(sock);
		#endif	
			if (ret < 0) {
				throw SocketCloseException(strerror(errno));
			}
			sock = -1;
		}
		return 0;
	}

	void UdpSocket::Bind(const IPAddress& ipAddress)
	{
		self_addr = ipAddress;
		self_addr_len = sizeof(self_addr);
		int opt = 1;
		int ret = ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
		if (ret < 0) {
			throw SocketException(strerror(errno));
		}
		ret = ::bind(sock, (sockaddr_t*)&self_addr, self_addr_len);
		if (ret < 0) {
			throw SocketBindingException(strerror(errno));
		}
		ret = ::getsockname(sock, (sockaddr_t*)&self_addr, &self_addr_len);
		if (ret < 0) {
			throw SocketException(strerror(errno));
		}
	}

	void UdpSocket::Bind(uint16_t portno)
	{
		auto ipaddr = IPAddress::Any(portno);
		this->Bind(ipaddr);
	}
	void UdpSocket::BindAny()
	{
		this->Bind(INPORT_ANY);
	}
	void UdpSocket::BindAny(uint16_t& portno)
	{
        this->Bind(INPORT_ANY);
		portno = IPAddress{self_addr}.port;
	}
	void UdpSocket::Connect(const IPAddress& ipaddr)
	{
		peer_addr = ipaddr;
		peer_addr_len = sizeof(peer_addr);
		int ret = ::connect(sock, (sockaddr_t*)&peer_addr, peer_addr_len);
		if (ret < 0) {
			throw SocketConnectException(strerror(errno));
		}
	}
	void UdpSocket::Connect(uint16_t portno)
	{
		auto ipaddr = IPAddress::Loopback(portno);
        this->Connect(ipaddr);
	}

	template <typename T>
	int UdpSocket::Send(const T& message, const IPAddress& ipaddr) const
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

	template <typename T>
	int UdpSocket::Receive(T& message, IPAddress& ipaddr) const
	{
		sockaddr_in_t addr_in;
		socklen_t addr_in_len = sizeof(addr_in);
		typename T::value_type buffer[10 * 1024];
		int ret = ::recvfrom(sock, (char*)buffer, sizeof(buffer), 0, (sockaddr_t*)&addr_in, &addr_in_len);
		
		if (ret < 0) {
			throw SocketReceiveException(strerror(errno));	
		}
		ipaddr = addr_in;
		message = { std::begin(buffer), std::begin(buffer)+ret};
		return ret;
	}
	int UdpSocket::Broadcast(int opt) const
	{
		int ret = ::setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&opt, sizeof(opt));
		if (ret < 0) {
			throw SocketException(strerror(errno));
		}
		return 0;
	}

	void UdpSocket::Interrupt() const
	{
		uint16_t portno = IPAddress{self_addr}.port;
		auto ipaddr = IPAddress::Loopback(portno);
		this->Send(msg_t{}, ipaddr);
	}
	
}
