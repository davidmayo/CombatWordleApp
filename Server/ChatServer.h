#pragma once

#include <WinSock2.h>

#include "Common/TcpEndpoint.h"
#include "Common/TcpServer.h"

#pragma comment(lib, "Ws2_32.lib")

class ChatServer : public TcpServer
{
public:
    ChatServer();
    ChatServer(
        int localOctet1,
        int localOctet2,
        int localOctet3,
        int localOctet4,
        int localPort
    );
    void SendString(std::string str);
    std::string ReceiveString();

    void Run();
};