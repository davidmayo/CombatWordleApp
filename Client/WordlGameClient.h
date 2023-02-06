#pragma once

#include "Common/TcpClient.h"
#include "Common/WordleGame.h"

namespace Wordle
{
    class WordleGameClient : public WordleGame
    {
    public:
        WordleGameClient();
        void SendGuess(std::string guess);
        std::string ReceiveGuess();
        void EstablishConnection();

    protected:
        TcpClient tcpClient;
    };
}
