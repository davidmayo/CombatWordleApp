#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <WinSock2.h>

// Have to undefine "max" after Windows import(s)
// since they define "max" as a macro,
// which conflicts with std::max() defined in <algorithm>
// See https://stackoverflow.com/questions/13416418/define-nominmax-using-stdmin-max#comment108173172_27522205
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

#pragma comment(lib, "Ws2_32.lib")

class TcpEndpoint
{
public:
	const static size_t MAX_BUFFER_SIZE = 1000;
	TcpEndpoint();
	TcpEndpoint(int octet1, int octet2, int octet3, int octet4, int port);
	~TcpEndpoint();
	void SetLocalSocket(int octet1, int octet2, int octet3, int octet4, int port);
	void CreateAndBindTcpSocket();
	std::string LocalSocketString() const;
	std::string ToString() const;
	
	void SendString(std::string);
	std::string ReceiveString();
	void Send(std::vector<char> buffer);
	void SendStream();
	std::vector<char> Receive();
	void ReceiveStream();
	std::string ReceiveStringFromStream();

protected:
	sockaddr local_sockaddr;
	sockaddr remote_sockaddr;
	SOCKET tcpSocket;
	WSADATA wsaData;

	OutputMemoryStream outputStream;
	InputMemoryStream inputStream;

	int localOctet1;
	int localOctet2;
	int localOctet3;
	int localOctet4;
	int localTcpPort;
};

std::ostream& operator<<(std::ostream& os, const TcpEndpoint& endpoint);
