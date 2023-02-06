#include "ChatServer.h"

ChatServer::ChatServer() : TcpServer()
{
}

ChatServer::ChatServer(
	int localOctet1,
	int localOctet2,
	int localOctet3,
	int localOctet4,
	int localPort
) : TcpServer(localOctet1, localOctet2, localOctet3, localOctet4, localPort)
{
}

void ChatServer::SendString(std::string str)
{
	std::vector<char> buffer;
	for (char ch : str)
	{
		buffer.push_back(ch);
	}
	this->Send(buffer);
}

std::string ChatServer::ReceiveString()
{
	//return this->ReceiveStringFromStream();
	std::vector<char> buffer = this->Receive();
	return std::string(buffer.begin(), buffer.end());
}

void ChatServer::Run()
{
	std::cout << "Server " << this->ToString() << " starting to listen . . . \n";
	this->Listen();

	std::cout << "Server " << this->ToString() << " ready to accept . . . \n";
	this->Accept();

	std::cout << "Server " << this->ToString() << " accepted a connection.\n";

	std::cout << "\n";
	std::cout << "Beginning chat session with [Server]. Type QUIT to end session.\n";

	while (true)
	{
		//std::cout << "Waiting for message from [Client] . . . \n";
		std::string message = this->ReceiveString();
		std::cout << "[Client]: " << message << "\n";
		if (message == "QUIT")
		{
			std::cout << "***** [Client] Ended session *****\n";
			return;
		}

		//std::cout << "Enter reply, or QUIT to exit:\n";
		std::cout << "[Server]: ";
		std::getline(std::cin, message);

		if (message == "QUIT")
		{
			std::cout << "***** [Server] Ended session *****\n";

			// Send termination message to distant end
			this->SendString("QUIT");
			return;
		}

		this->SendString(message);
	}
}
