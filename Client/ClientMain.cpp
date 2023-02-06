#include <iostream>

#include "ChatClient.h"
#include "WordlGameClient.h"

#include "Common/TcpEndpoint.h"
#include "Common/TcpClient.h"

int main()
{
	std::cout << "Creating client\n";
	Wordle::WordleGameClient client;
	std::cout << "DONE Creating client\n";

	std::cout << "Calling Run()\n";
	client.Run();
	std::cout << "DONE Calling Run()\n";

	return 0;

	std::cout << "*************************\n";
	std::cout << "*      CHAT CLIENT      *\n";
	std::cout << "*************************\n";
	std::cout << "\n";
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	std::cout << "!!!!!      SET UP THE SERVER FIRST      !!!!!\n";
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
	std::cout << "\n";
	std::cout << "Pausing now. Hit enter once server is set up.\n";
	std::cin.get();

	int localOctet1, localOctet2, localOctet3, localOctet4, localTcpPort;
	std::cout << "Configure client.\n";
	std::cout << "Suggested local socket (loopback address): IPv4 address = 127.127.127.127, TCP port 54321\n";
	std::cout << "Enter local IPv4 address, with spaces between: ";
	std::cin >> localOctet1 >> localOctet2 >> localOctet3 >> localOctet4;
	std::cout << "Enter local TCP port: ";
	std::cin >> localTcpPort;
	std::cout << "\n";
	
	int remoteOctet1, remoteOctet2, remoteOctet3, remoteOctet4, remoteTcpPort;
	std::cout << "Configure server.\n";
	std::cout << "Suggested remote socket (loopback address): IPv4 address = 127.0.0.1, TCP port 1111\n";
	std::cout << "Enter server IPv4 address, with spaces between: ";
	std::cin >> remoteOctet1 >> remoteOctet2 >> remoteOctet3 >> remoteOctet4;
	std::cout << "Enter server TCP port: ";
	std::cin >> remoteTcpPort;

	std::cin.ignore();

	ChatClient chatClient(
		localOctet1,  localOctet2,  localOctet3,  localOctet4,  localTcpPort,
		remoteOctet1, remoteOctet2, remoteOctet3, remoteOctet4, remoteTcpPort
	);

	chatClient.Run();

	std::cout << "Application terminated.\n";
	return 0;
}