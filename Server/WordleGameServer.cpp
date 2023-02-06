#include "WordleGameServer.h"

Wordle::WordleGameServer::WordleGameServer()
	: WordleGame(),
	tcpServer(127, 0, 0, 1, 1111)	// Hard-coding server port 127.0.0.1:1111
									// since this is a proof-of-concept
{
	this->SetPlayerId(Wordle::Player::Server);
}

void Wordle::WordleGameServer::SendGuess(std::string guess)
{
	std::string sendString = "=" + guess;
	this->tcpServer.SendString(sendString);
}

std::string Wordle::WordleGameServer::ReceiveGuess()
{
	std::string receivedString = this->tcpServer.ReceiveString();
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

		// On restart, server goes first.
		// So get user input, and send it.
		std::string newGuess = this->GetUserGuess();
		this->SendGuess(newGuess);

		// Return the new guess
		return newGuess;
	}
	else
	{
		// Invalid input;
		std::cout << "ERROR! Could not interpret string from distant end \""
			<< receivedString << "\". Fatal error. Exiting program.\n";
		exit(1);
	}

	return word;
	//return this->tcpServer.ReceiveString();
}

void Wordle::WordleGameServer::EstablishConnection()
{
	std::cout << "Server " << this->tcpServer.ToString() << " starting to listen . . . \n";
	this->tcpServer.Listen();

	std::cout << "Server " << this->tcpServer.ToString() << " ready to accept . . . \n";
	this->tcpServer.Accept();

	std::cout << "Server " << this->tcpServer.ToString() << " accepted a connection.\n";
	
	//// Send the RESET command
	//std::string command = "!" + this->goalWord;
	////std::cout << "DEBUG: Sending command: " << command << "\n";
	//this->tcpServer.SendString(command);
	////std::cin.get();

	this->isConnectionEstablished = true;
}
