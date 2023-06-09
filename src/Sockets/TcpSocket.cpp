#include <Sockets/Exceptions.hpp>
#include <Sockets/TcpSocket.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

#define DEBUG_ACT true

namespace Socket {
	void DEBUG(std::string message)
	{
		if (DEBUG_ACT)
			std::cout << "DEBUG: " << message << std::endl;
	}

	TcpSocket::TcpSocket()
	{
		mInfo = new addrinfo;
		memset(mInfo, 0, sizeof *mInfo);
		mInfo->ai_family = AF_UNSPEC;
		mInfo->ai_socktype = SOCK_STREAM;
	}

	TcpSocket::TcpSocket(int family, int flags)
	{
		mInfo = new addrinfo;
		memset(mInfo, 0, sizeof *mInfo);
		mInfo->ai_family = family;
		mInfo->ai_socktype = SOCK_STREAM;
		mInfo->ai_flags = flags;

		if (family == AF_UNSPEC)
			return; // Can't create socket now


		mSock = socket(mInfo->ai_family, mInfo->ai_socktype, 0);
		if (mSock == -1)
			throw SocketCreationException(strerror(errno));
		// Socket successfully "opened"
		mSockCreated = true;
	}

	TcpSocket::TcpSocket(int socket, addrinfo info, bool bound, bool connected)
		: mSock(socket), mBound(bound), mConnected(connected)
	{
		mInfo = new addrinfo(info);
	}

	TcpSocket::~TcpSocket()
	{
		if (mSockCreated && !mClosed)
			Close();
		freeaddrinfo(mInfo);
	}

	void TcpSocket::Bind(uint16_t port)
	{
		if (mBound && mConnected)
			throw SocketBindingException("Already bound!");

		setInfo(port);

		addrinfo * result;

		for (result = mInfo; result != NULL; result = mInfo->ai_next)
		{
			if (!mSockCreated)
			{
				try {
					openSocket(result);
				} catch(SocketCreationException &e) {
					continue;
				}
			}

			// Socket successfully opened from here
			if (::bind(mSock, result->ai_addr, result->ai_addrlen) == 0)
			{
				mBound = true;
				return;
			}
		}
		// Couldn't bind, throw
		throw SocketBindingException("Can't bind to port");
	}

    void TcpSocket::BindAny() {
        Bind(INPORT_ANY);
    }
    void TcpSocket::BindAny(uint16_t& port)
    {
        this->Bind(INPORT_ANY);
        //portno = port;
    }


    void TcpSocket::Connect(const IPAddress &ipaddr) {
        Connect(ipaddr.addr_string(), ipaddr.port);
    }

	void TcpSocket::Connect(std::string address, uint16_t port)
	{
		if (mConnected)
			throw SocketConnectException("Already Connected!");

		setInfo(address, htons(port));

		addrinfo * result;
		for (result = mInfo; result!= NULL; result = mInfo->ai_next)
		{
			if (!mSockCreated) {
				try {
					openSocket(result);
				} catch (SocketCreationException &e) {
					continue;
				}
			}
			// Socket successfully opened from here
			if (::connect(mSock, result->ai_addr, result->ai_addrlen) == 0)
			{
				mConnected = true;
				return;
			}
		}
		throw SocketConnectException("Can't connect to host");
	}

	void TcpSocket::Listen(int maxQueue)
	{
		if (::listen(mSock, maxQueue) != 0)
			throw SocketListenException(strerror(errno));
		DEBUG("Listening...");
	}

	std::shared_ptr<TcpSocket> TcpSocket::Accept()
	{
		DEBUG("Starting to accept");
		union
		{
			sockaddr addr;
			sockaddr_in in;
			sockaddr_in6 in6;
			sockaddr_storage s;
		} address;
		socklen_t addressSize = sizeof(sockaddr_storage);
		int newSock;
		if ((newSock = ::accept(mSock, (struct sockaddr*)0, (unsigned int*)0))==-1) {
		//if ((newSock = ::accept(mSock, (sockaddr*)&address.s, &addressSize)) == -1) {
			DEBUG(strerror(errno));
			throw SocketAcceptException(strerror(errno));

		DEBUG("1 client accepted");

		addrinfo info;
		memset(&info, 0, sizeof info);
		if (address.s.ss_family == AF_INET)
		{
			info.ai_family = AF_INET;
			info.ai_addr = new sockaddr(address.addr);
		} else {
			info.ai_family = AF_INET6;
			info.ai_addr = new sockaddr(address.addr);
		}
		return std::shared_ptr<TcpSocket>(new TcpSocket(newSock, info, true, false));
	}

	void TcpSocket::Send(const char *data, unsigned int length, int flags)
	{
		const char * buff = data;
		int status = 0;
		int total_sent = 0;
		int left_to_send = length;
		while(total_sent < length)
		{
			status = ::send(mSock, buff + total_sent, left_to_send, flags);
			if (status == -1)
			{
				throw SocketSendException(strerror(errno));
			} else {
				total_sent += status;
				left_to_send -= status;
			}
		}
	}

	bool TcpSocket::Receive(char* msg, int len, int flags)
	{
		int status;
		if ((status = ::recv(mSock, msg, len, flags)) == -1)
			throw SocketReceiveException(strerror(errno));
		else if (status == 0)
			return false;

		return true;
	}

	void TcpSocket::Close()
	{
		if (::close(mSock) == -1)
			throw SocketCloseException(strerror(errno));
		else
			mClosed = true;
	}
	// Private:
	void TcpSocket::setInfo(int port)
	{
		setInfo("null", port);
	}
	void TcpSocket::setInfo(std::string address, int port)
	{
		const char *charAddress;
		if (address == "null")
			charAddress = NULL;
		else
			charAddress = address.c_str();

		addrinfo hints = *mInfo;
		int status;
		if ((status = getaddrinfo(charAddress, std::to_string(port).c_str(), &hints, &mInfo)) != 0)
		{
			delete charAddress;
			throw SocketException(
			("getaddrinfo returned non-zero: " + std::string(gai_strerror(status))).c_str()
			);			
		}
		delete charAddress;
	}

	void TcpSocket::openSocket(addrinfo * info)
	{
		mSock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if (mSock == -1)
		{
			SocketCreationException except(strerror(errno));
			throw except;
		}
	}
}
