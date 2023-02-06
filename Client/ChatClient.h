#pragma once

#include <WinSock2.h>

#include "Common/TcpEndpoint.h"
#include "Common/TcpClient.h"

#pragma comment(lib, "Ws2_32.lib")

class ChatClient : public TcpClient
{
public:
    ChatClient();
    ChatClient(
        int localOctet1,
        int localOctet2,
        int localOctet3,
        int localOctet4,
        int localPort
    );
    ChatClient(
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
    );
    void SendString(std::string str);
    std::string ReceiveString();

    void Run();

protected:

private:

};