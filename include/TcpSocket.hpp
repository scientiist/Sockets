#pragma once

#include <iostream>
#include <string>
#include <netdb.h>
#include <memory>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "IPAddress.hpp"

#ifndef INPORT_ANY
#define INPORT_ANY
#endif

namespace Socket {
	class TcpSocket {
	public:
		TcpSocket();
		TcpSocket(int family, int flags);
		TcpSocket(int socket, addrinfo info, bool connected, bool bound);
		virtual ~TcpSocket();

		TcpSocket(const TcpSocket &socket) = default;
		TcpSocket &operator=(const TcpSocket &socket) = delete;
		void Bind(int port);
		void Connect(std::string address, int port);
		void Listen(int maxQueue);
		std::shared_ptr<TcpSocket> Accept();
		void Send(const char *data, unsigned int length, int flags);
		// Receive data (blocking)
		// @return true if socket is still open, false otherwise
		bool Receive(char* msg, int len, int flags);
		void Close();
	private:
		void setInfo(int port);
		void setInfo(std::string address, int port);
		void openSocket(addrinfo *info);
		addrinfo * mInfo;
		int mSock = -1;
		bool mSockCreated = false;
		bool mBound = false;
		bool mConnected = false;
		bool mClosed = false;
	};
}
