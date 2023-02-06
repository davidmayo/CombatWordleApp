#include "TcpServer.h"

TcpServer::TcpServer() : TcpEndpoint()
{
}

TcpServer::TcpServer(
	int localOctet1,
	int localOctet2,
	int localOctet3,
	int localOctet4,
	int localPort
) : TcpEndpoint(localOctet1, localOctet2, localOctet3, localOctet4, localPort)
{
}

std::string TcpServer::ToString() const
{
	std::stringstream ss;

	ss << "<TcpServer LOCAL: "
		<< this->LocalSocketString()
		<< ">";
	return ss.str();
}

void TcpServer::Listen()
{
	//std::cout << "DEBUG: TcpServer()::Listen() called\n";
	int result = listen(this->tcpSocket, 1);
	//std::cout << "DEBUG: listen() result=" << result;
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << " ERROR: " << error << "\n";
	}
	else
	{
		//std::cout << "DEBUG: NO ERROR" << "\n";
	}
	//std::cout << "DEBUG: TcpServer()::Listen() exiting\n";

}

void TcpServer::Accept()
{
	//std::cout << "DEBUG: TcpServer()::Accept() called\n";
	sockaddr sender;
	int size = sizeof(sockaddr);
	SOCKET acceptedConnectionSocket = accept(this->tcpSocket, &sender, &size);
	//std::cout << "DEBUG: accept() result="<< acceptedConnectionSocket << " ";
	if (acceptedConnectionSocket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		std::cout << "ERROR: " << error << "\n";
	}
	else
	{
		//std::cout << "DEBUG: NO ERROR" << "\n";
	}
	
	this->tcpSocket = acceptedConnectionSocket;
	//std::cout << "DEBUG: TcpServer()::Accept() exiting\n";
}

std::ostream& operator<<(std::ostream& os, const TcpServer& server)
{
	os << server.ToString();
	return os;
}
