#pragma once
#include "TcpEndpoint.h"
class TcpServer :
    public TcpEndpoint
{
public:
    TcpServer();
    TcpServer(
        int localOctet1,
        int localOctet2,
        int localOctet3,
        int localOctet4,
        int localPort
    );
    std::string ToString() const;
    void Listen();
    void Accept();
protected:
};

std::ostream& operator<<(std::ostream& os, const TcpServer& server);
