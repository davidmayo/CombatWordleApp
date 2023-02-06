#pragma once

#include "Common/TcpServer.h"
#include "Common/WordleGame.h"

namespace Wordle
{
    class WordleGameServer : public WordleGame
    {
    public:
        WordleGameServer();
        void SendGuess(std::string guess);
        std::string ReceiveGuess();
        void EstablishConnection();

    protected:
        TcpServer tcpServer;
    };
}