#pragma once
#include "TcpEndpoint.h"
class TcpClient :
    public TcpEndpoint
{
public:
    TcpClient();
    TcpClient(
        int localOctet1,
        int localOctet2,
        int localOctet3,
        int localOctet4,
        int localPort
    );
    TcpClient(
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
    std::string ToString() const;
    std::string RemoteSocketString() const;

    void SetRemoteSocket(
        int remoteOctet1,
        int remoteOctet2,
        int remoteOctet3,
        int remoteOctet4,
        int remotePort
    );

    void Connect();
protected:
    int remoteOctet1;
    int remoteOctet2;
    int remoteOctet3;
    int remoteOctet4;
    int remotePort;

    sockaddr_in remote_sockaddr_in;
    sockaddr remote_sockaddr;
};

std::ostream& operator<<(std::ostream& os, const TcpClient& client);

