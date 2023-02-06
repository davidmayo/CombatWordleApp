#include "TcpEndpoint.h"

TcpEndpoint::TcpEndpoint() : TcpEndpoint(0, 0, 0, 0, 0)
{
}

TcpEndpoint::TcpEndpoint(int octet1, int octet2, int octet3, int octet4, int port)
	: inputStream(nullptr, 64), outputStream()
{
	//std::cout << "DEBUG: In TcpEndpointConstructor()\n";
	// Startup Winsock library
	
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		// TODO: Handle error
	}

	this->SetLocalSocket(octet1, octet2, octet3, octet4, port);
	this->CreateAndBindTcpSocket();

	//this->outputStream;
	//this->inputStream = InputMemoryStream(nullptr, 64);
}

TcpEndpoint::~TcpEndpoint()
{
	WSACleanup();
}

void TcpEndpoint::SetLocalSocket(int octet1, int octet2, int octet3, int octet4, int port)
{
	sockaddr_in localAddrIn;
	localAddrIn.sin_family = AF_INET;	// IPv4
	localAddrIn.sin_port = htons(port);
	localAddrIn.sin_addr.S_un.S_un_b.s_b1 = octet1;
	localAddrIn.sin_addr.S_un.S_un_b.s_b2 = octet2;
	localAddrIn.sin_addr.S_un.S_un_b.s_b3 = octet3;
	localAddrIn.sin_addr.S_un.S_un_b.s_b4 = octet4;

	// Convert to sockaddr
	memcpy(&local_sockaddr, &localAddrIn, sizeof(local_sockaddr));

	this->localOctet1 = octet1;
	this->localOctet2 = octet2;
	this->localOctet3 = octet3;
	this->localOctet4 = octet4;
	this->localTcpPort = port;
}

void TcpEndpoint::CreateAndBindTcpSocket()
{
	tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	bind(tcpSocket, &local_sockaddr, sizeof(sockaddr));
}

/// <summary>
/// Get socket as a string like "127.0.0.1:80"
/// </summary>
/// <returns></returns>
std::string TcpEndpoint::LocalSocketString() const
{
	std::stringstream ss;

	ss  << this->localOctet1 << '.'
		<< this->localOctet2 << '.'
		<< this->localOctet3 << '.'
		<< this->localOctet4
		<< ':' << this->localTcpPort;
	return ss.str();
}

std::string TcpEndpoint::ToString() const
{
	std::stringstream ss;

	ss  << "<TcpEndpoint LOCAL: "
		<< this->LocalSocketString()
		<< ">";
	return ss.str();
}

void TcpEndpoint::SendString(std::string str)
{
	std::vector<char> buffer;
	for (char ch : str)
		buffer.push_back(ch);

	this->Send(buffer);
}

std::string TcpEndpoint::ReceiveString()
{
	std::vector<char> buffer = this->Receive();
	return std::string(buffer.begin(), buffer.end());
}

void TcpEndpoint::Send(std::vector<char> buffer)
{
	//std::cout << "DEBUG: TcpEndpoint::Send(): buffer=[";
	//for (char ch : buffer)
	//{
	//	std::cout << ch;
	//}
	//std::cout << "]\n";

	//// Write this buffer to the output stream
	//this->outputStream.Write(buffer);

	//// Send the output stream over TCP
	//this->SendStream();

	
	//std::cout << "DEBUG: TcpEndpoint::Send() invoked\n";
	
	if (buffer.size() > MAX_BUFFER_SIZE)
	{
		std::cout << "ERROR! Max buffer size is " << MAX_BUFFER_SIZE << " bytes. "
			<< "Cannot transmit " << buffer.size() << " bytes. No action taken.\n";
	}
	else
	{
		//std::cout << "DEBUG: MAX_BUFFER_SIZE=" << MAX_BUFFER_SIZE
		//	<< "; buffer.size()=" << buffer.size() << " OK\n";
	}

	char send_buffer[MAX_BUFFER_SIZE];
	std::copy(buffer.begin(), buffer.end(), send_buffer);

	//std::cout << "Successfully copied\n";

	int bytesSent = send(this->tcpSocket, send_buffer, buffer.size(), 0);

	if (bytesSent == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << "ERROR: TcpEndpoint::send() error=" << error << "\n";
	}
	else
	{
		//std::cout << "DEBUG: TcpEndpoint::send() sent " << bytesSent << " bytes.\n";
	}

	//std::cout << "DEBUG: TcpEndpoint::Send() exiting\n";
	
}

void TcpEndpoint::SendStream()
{
	//std::vector<char> streamCopy = {};
	//streamCopy.resize(stream.GetLength());

	char* streamCopy = (char*)std::malloc(this->outputStream.GetLength());
	std::memcpy(streamCopy, this->outputStream.GetBufferPtr(), this->outputStream.GetLength());

	std::vector<char> streamCopyVector(streamCopy, streamCopy + this->outputStream.GetLength());

	std::cout << "DEBUG: TcpEndpoint::SendStream()\n";
	std::cout << "DEBUG: streamCopyVector is\n";
	for (int index = 0; index < streamCopyVector.size(); ++index)
	{
		char ch = streamCopyVector[index];
		std::cout << "\ti=" << index << "\t" << ch << "\t" << (int)ch << "\n";
	}
	std::cout << "\n";

	//Send(streamCopyVector);

	//std::cout << "DEBUG: TcpEndpoint::Send() invoked\n";

	if (streamCopyVector.size() > MAX_BUFFER_SIZE)
	{
		std::cout << "ERROR! Max buffer size is " << MAX_BUFFER_SIZE << " bytes. "
			<< "Cannot transmit " << streamCopyVector.size() << " bytes. No action taken.\n";
	}
	else
	{
		//std::cout << "DEBUG: MAX_BUFFER_SIZE=" << MAX_BUFFER_SIZE
		//	<< "; buffer.size()=" << buffer.size() << " OK\n";
	}

	char send_buffer[MAX_BUFFER_SIZE];
	std::copy(streamCopyVector.begin(), streamCopyVector.end(), send_buffer);

	//std::cout << "Successfully copied\n";

	int bytesSent = send(this->tcpSocket, send_buffer, streamCopyVector.size(), 0);

	if (bytesSent == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << "ERROR: TcpEndpoint::send() error=" << error << "\n";
	}
	else
	{
		//std::cout << "DEBUG: TcpEndpoint::send() sent " << bytesSent << " bytes.\n";
	}

	//std::cout << "DEBUG: TcpEndpoint::Send() exiting\n";
}

std::vector<char> TcpEndpoint::Receive()
{
	char buffer[MAX_BUFFER_SIZE];
	int receivedSize = recv(this->tcpSocket, buffer, sizeof(buffer), 0);

	if (receivedSize == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << "ERROR: TcpEndpoint::Receive() error=" << error << "\n";
		return {};
	}
	else
	{
		//std::cout << "DEBUG: TcpEndpoint::Receive() received " << receivedSize << " bytes\n";
		//for (int i = 0; i < receivedSize; i++)
		//{
		//	std::cout << "\ti=" << i << "\t" << buffer[i] << "\n";
		//}
	}

	std::vector<char> returnValue;
	for (int i = 0; i < receivedSize; i++)
	{
		returnValue.push_back( buffer[i] );
	}
	return returnValue;
}

std::string TcpEndpoint::ReceiveStringFromStream()
{
	std::cout << "DEBUG: TcpEndpoint::ReceiveStringFromStream()\n";
	this->ReceiveStream();

	std::string returnValue;
	this->inputStream.ReadString(returnValue);

	return returnValue;
}

void TcpEndpoint::ReceiveStream()
{
	std::cout << "DEBUG: TcpEndpoint::ReceiveStream()\n";

	// Receive the raw bytes
	char buffer[MAX_BUFFER_SIZE];
	int receivedSize = recv(this->tcpSocket, buffer, sizeof(buffer), 0);

	if (receivedSize == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cout << "ERROR: TcpEndpoint::Receive() error=" << error << "\n";
		return;
	}
	else
	{
		std::cout << "DEBUG: TcpEndpoint::ReceiveStream() received " << receivedSize << " bytes\n";
		for (int i = 0; i < receivedSize; i++)
		{
			std::cout << "\ti=" << i << "\t" << buffer[i] << "\t" << (int)buffer[i] << "\n";
		}
	}

	this->inputStream = InputMemoryStream(buffer, receivedSize);
	//std::vector<char> returnValue;
	//this->inputStream.Read(returnValue);
	//return returnValue;

	/*for (int i = 0; i < receivedSize; i++)
	{
		returnValue.push_back(buffer[i]);
	}
	return returnValue;*/
}

std::ostream& operator<<(std::ostream& os, const TcpEndpoint& endpoint)
{
	os << endpoint.ToString();
	return os;
}