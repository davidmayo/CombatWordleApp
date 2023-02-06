#include <iostream>

#include "Common/TcpServer.h"
#include "ChatServer.h"
#include "WordleGameServer.h"

int main()
{
	std::cout << "SERVER\n";
	std::cout << "Creating server\n";
	Wordle::WordleGameServer server;
	std::cout << "DONE Creating server\n";

	std::cout << "Run()\n";
	server.Run();
	std::cout << "DONE Run()\n";

	return 0;
	std::cout << "*************************\n";
	std::cout << "*      CHAT SERVER      *\n";
	std::cout << "*************************\n";
	std::cout << "\n";

	int localOctet1, localOctet2, localOctet3, localOctet4, localTcpPort;
	std::cout << "Configure server local socket.\n";
	std::cout << "Suggested server local socket (loopback address): IPv4 address = 127.0.0.1, TCP port 1111.\n";
	std::cout << "Enter local IPv4 address, with spaces between: ";
	std::cin >> localOctet1 >> localOctet2 >> localOctet3 >> localOctet4;
	std::cout << "Enter local TCP port: ";
	std::cin >> localTcpPort;
	std::cout << "\n";
	std::cin.ignore();

	ChatServer chatServer(
		localOctet1, localOctet2, localOctet3, localOctet4, localTcpPort
	);

	chatServer.Run();

	std::cout << "Application terminated.\n";
	return 0;
}