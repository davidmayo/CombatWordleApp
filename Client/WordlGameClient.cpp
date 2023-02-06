#include "WordlGameClient.h"

Wordle::WordleGameClient::WordleGameClient() :
	WordleGame(),
	tcpClient(127, 127, 127, 127, 54321,	// Hardcoding client socket 127.127.127.127:54321
		      127,   0,   0,   1,  1111)	// Hardcoding server socket 127.0.0.1:1111
{
	this->SetPlayerId(Wordle::Player::Client);
}

void Wordle::WordleGameClient::SendGuess(std::string guess)
{
	std::string sendString = "=" + guess;
	this->tcpClient.SendString(sendString);
}

std::string Wordle::WordleGameClient::ReceiveGuess()
{
	std::string receivedString = this->tcpClient.ReceiveString();
	char controlChar = receivedString[0];
	std::string word = receivedString.substr(1, 5);

	if (controlChar == '=')
	{
		// Regular guess command
		return word;
	}
	else if (controlChar == '!')
	{
		// Game reset command
		this->goalWord = word;

		//std::cout << "DEBUG: Resetting to word " << this->goalWord << "\n";
		//std::cin.get();

		this->Initialize();

		// On restart, server goes first. So wait for their input
		// by calling this function recursively
		std::string receivedGuess = this->ReceiveGuess();
		this->errorMessage = "RESET TO " + this->goalWord;

		return receivedGuess;
	}
	else
	{
		// Invalid input
		std::cout << "ERROR! Could not interpret string from distant end \""
			<< receivedString << "\".\n";
		exit(1);
	}
}

void Wordle::WordleGameClient::EstablishConnection()
{
	// TODO
	std::cout << "DEBUG: void Wordle::WordleGameClient::EstablishConnection()\n";

	std::cout << "DEBUG: Local IP is " << tcpClient.LocalSocketString() << "\n";
	std::cout << "\n";
	std::cout << "Hit enter to connect to " << this->tcpClient.RemoteSocketString() << " . . . ";
	std::cin.get();

	std::cout << "Attempting to connect . . . ";
	this->tcpClient.Connect();
	std::cout << "SUCCESS\n";

	this->isConnectionEstablished = true;
	
	//// Send reset command
	//std::string command = "!" + this->goalWord;
	//this->tcpClient.SendString(command);
}
