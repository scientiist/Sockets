#include "Sockets/TcpSocket.hpp"

#include <iostream>
#include <exception>
#include <cstring>
#include <memory>

using namespace Socket;
using Socket_p = std::shared_ptr<TcpSocket>;

int main(int argc, char *argv[])
{
	Socket_p sock(new TcpSocket);
	Socket_p client;

	try {
		sock->Bind(42069);
		sock->Listen(5);
		client = sock->Accept();
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	// Welcoming the new user
	client->Send("Welcome !\n\f", 15, 0);
	// Closing the listening socket, we want nobody else.
	sock->Close();

	char data[512];
	memset(&data, 0, 512);
	while (client->Receive(data, sizeof data, 0))
	{
		std::cout << "[Recv] " << data << std::endl;
		client->Send(data, sizeof data, 0);
		memset(&data, 0, 512);
	}
	client->Close();

	return 0;
}
