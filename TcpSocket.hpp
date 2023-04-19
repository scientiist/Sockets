#pragma once

#include <iostream>
#include <string>
#include <netdb.h>
#include <memory>

namespace Socket {

	class SocketException : public std::exception {
	protected:
		char * message;
	public:
		SocketException(std::string msg) : message(msg.c_str()) {}
		SocketException(char *msg) : message(msg) {}
		char * what() {
			return message;
		}	
	};

	class SocketBindingException : public SocketException {
	public:
		SocketBindingException(char *msg) : SocketException{msg} {}
	};

	class SocketConnectException : public SocketException {
	public:
		SocketConnectException(char *msg) : SocketException{msg} {}
	};

	class SocketCreationException : public SocketException {
	public:	
		SocketCreationException(char * msg) : SocketException{msg} {}
	};

	class SocketListenException : public SocketException {
	public:
		SocketListenException(char * msg) : SocketException{msg} {}
	};

	class SocketAcceptException : public SocketException {
	public:
		SocketAcceptException(char * msg) : SocketException{msg} {}
	};

	class SocketSendException : public SocketException {
	public:
		SocketSendException(char * msg) : SocketException{msg} {}
	};

	class SocketReceiveException : public SocketException {
	public:
		SocketReceiveException(char * msg) : SocketException{msg} {}
	};

	class SocketCloseException : public SocketException {
	public:
		SocketCloseException(char * msg) : SocketException{msg} {}
	};

	
	class TcpSocket {
	public:
		TcpSocket();
		TcpSocket(int family, int flags);
		TcpSocket(int socket, addrinfo info, bool connected, bool bound);
		virtual ~TcpSocket();

		TcpSocket(const TcpSocket &socket) = default;
		TcpSocket &operator=(const TcpSocket &socket) = delete;
		void bind(int port);
		void connect(std::string address, int port);
		void listen(int maxQueue);
		std::shared_ptr<TcpSocket> accept();
		void send(const char *data, unsigned int length, int flags);
		// Receive data (blocking)
		// @return true if socket is still open, false otherwise
		bool receive(char* msg, int len, int flags);
		void close();
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
