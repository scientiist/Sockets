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
}
