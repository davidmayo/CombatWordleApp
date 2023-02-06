#include "ChatClient.h"

ChatClient::ChatClient() : TcpClient()
{
}

ChatClient::ChatClient(
	int localOctet1,
	int localOctet2,
	int localOctet3,
	int localOctet4,
	int localPort
) : TcpClient(localOctet1, localOctet2, localOctet3, localOctet4, localPort)
{
}

ChatClient::ChatClient(
	int localOctet1,
	int localOctet2,
	int localOctet3,
	int localOctet4,
	int localPort,
	int remoteOctet1,
	int remoteOctet2,
	int remoteOctet3,
	int remoteOctet4,
	int remotePort
) : TcpClient(localOctet1, localOctet2, localOctet3, localOctet4, localPort,
	remoteOctet1, remoteOctet2, remoteOctet3, remoteOctet4, remotePort)
{
}

void ChatClient::SendString(std::string str)
{
	std::vector<char> buffer;
	for (char ch : str)
		buffer.push_back(ch);
	
	this->Send(buffer);
}

std::string ChatClient::ReceiveString()
{
	//return this->ReceiveStringFromStream();

	std::vector<char> buffer = this->Receive();
	return std::string(buffer.begin(), buffer.end());
}

void ChatClient::Run()
{
	std::cout << "\n";
	std::cout << "Hit enter to connect to " << this->RemoteSocketString() << " . . . ";
	std::cin.get();

	std::cout << "Attempting to connect . . . ";
	this->Connect();
	std::cout << "SUCCESS\n";

	std::string message;

	std::cout << "\n";
	std::cout << "Beginning chat session with [Server]. Type QUIT to end session.\n";

	// Main loop. Repeat forever, until receive "QUIT" from local or remote end.
	while (true)
	{
		std::cout << "[Client]: ";
		std::getline(std::cin, message);

		if (message == "QUIT")
		{
			std::cout << "***** [Client] Ended session *****\n";

			// Send quit message to distant end
			this->SendString("QUIT");
			return;
		}

		this->SendString(message);

		//std::cout << "Awaiting response . . . \n";

		message = this->ReceiveString();

		// Quit checking from distant end
		if (message == "QUIT")
		{
			std::cout << "***** [Server] Ended session *****\n";
			return;
		}

		std::cout << "[Server]: " << message << "\n";
	}
}

