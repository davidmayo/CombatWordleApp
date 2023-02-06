#include "TcpClient.h"

TcpClient::TcpClient() : TcpEndpoint()
{
}

TcpClient::TcpClient(
	int localOctet1,
	int localOctet2,
	int localOctet3,
	int localOctet4,
	int localPort
) : TcpEndpoint(localOctet1, localOctet2, localOctet3, localOctet4, localPort)
{
}

TcpClient::TcpClient(
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
) : TcpEndpoint(localOctet1, localOctet2, localOctet3, localOctet4, localPort)
{
	this->SetRemoteSocket(remoteOctet1, remoteOctet2, remoteOctet3, remoteOctet4, remotePort);
	//this->remoteOctet1 = remoteOctet1;
	//this->remoteOctet2 = remoteOctet2;
	//this->remoteOctet3 = remoteOctet3;
	//this->remoteOctet4 = remoteOctet4;
	//this->remotePort = remotePort;
}

std::string TcpClient::ToString() const
{
	std::stringstream ss;

	ss << "<TcpClient LOCAL: "
		<< this->LocalSocketString()
		<< ", REMOTE: " << this->RemoteSocketString()
		<< ">";
	return ss.str();
}

std::string TcpClient::RemoteSocketString() const
{
	std::stringstream ss;

	ss << this->remoteOctet1 << '.'
		<< this->remoteOctet2 << '.'
		<< this->remoteOctet3 << '.'
		<< this->remoteOctet4
		<< ':' << this->remotePort;
	return ss.str();
}

void TcpClient::SetRemoteSocket(int remoteOctet1, int remoteOctet2, int remoteOctet3, int remoteOctet4, int remotePort)
{
	this->remoteOctet1 = remoteOctet1;
	this->remoteOctet2 = remoteOctet2;
	this->remoteOctet3 = remoteOctet3;
	this->remoteOctet4 = remoteOctet4;
	this->remotePort = remotePort;

	this->remote_sockaddr_in.sin_family = AF_INET;	// IPv4
	this->remote_sockaddr_in.sin_port = htons(this->remotePort);	
	this->remote_sockaddr_in.sin_addr.S_un.S_un_b.s_b1 = this->remoteOctet1;
	this->remote_sockaddr_in.sin_addr.S_un.S_un_b.s_b2 = this->remoteOctet2;
	this->remote_sockaddr_in.sin_addr.S_un.S_un_b.s_b3 = this->remoteOctet3;
	this->remote_sockaddr_in.sin_addr.S_un.S_un_b.s_b4 = this->remoteOctet4;

	std::cout << "DEBUG: TcpClient::SetRemoteSocket()" << "\n";

	// Copy socket information from sockaddr_in to sockaddr
	memcpy(&(this->remote_sockaddr), &(this->remote_sockaddr_in), sizeof(this->remote_sockaddr));
}

void TcpClient::Connect()
{
	// Intitialize TCP handshake (SYN)
	int result = connect(this->tcpSocket, &(this->remote_sockaddr), sizeof(sockaddr));
	//std::cout << "DEBUG: TcpClient::Connect() result=" << result;

	if (result != 0)
	{
		int error = WSAGetLastError();
		std::cout << " error=" << error << "\n";
	}
	else
	{
		std::cout << " NO ERROR\n";
	}
}

std::ostream& operator<<(std::ostream& os, const TcpClient& client)
{
	os << client.ToString();
	return os;
}
