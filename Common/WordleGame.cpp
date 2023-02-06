#include "WordleGame.h"


Wordle::WordleGame::WordleGame()
{
    // Read the file of possible solution words
    std::vector<std::string> answer_words = {};
    std::string token;
    std::ifstream inStream("../word_lists/answer_words_clean.txt");
    while (std::getline(inStream, token))
    {
        //std::cout << token << "\n";
        answer_words.push_back(token);
    }
    inStream.close();

    // Choose a random word
    std::srand(time(NULL));
    size_t randomIndex = (size_t)(std::rand()) % answer_words.size();
    this->goalWord = answer_words[randomIndex];

    //std::cout << "Index=" << randomIndex << ", word=" << this->goalWord << "\n";
    //std::cout << "PAUSE . . . ";
    //std::cin >> token;
    this->isConnectionEstablished = false;

    Initialize();
}

Wordle::WordleGame::WordleGame(std::string goalWord) : console()
{
    this->goalWord = goalWord;
    this->isConnectionEstablished = false;
    Initialize();

}

void Wordle::WordleGame::Initialize()
{
    this->guesses = {};

    this->incorrectLetters = {};
    this->wrongPlaceLetters = {};
    this->correctLetters = {};

    this->isGameOver = false;

    this->playerId = Player::NotSet;

    this->errorMessage = "";

    for (size_t index = 0; index < 26; ++index)
        this->keyboardStatuses[index] = LetterStatus::Undetermined;

    // Load the valid guess words from file
    this->legalGuesses = {};
    std::string token;
    std::ifstream inStream("../word_lists/guess_words_clean.txt");
    while (std::getline(inStream, token))
    {
        //std::cout << token << "\n";
        this->legalGuesses.push_back(token);
    }
    inStream.close();
}

//void Wordle::WordleGame::GuessWord(std::string guess)
//{
//    //// Validate the guess input
//    //
//    //// 5 letter words only
//    //if (guess.size() != 5)
//    //    return;
//
//    //// Make sure all chars are letters, and convert to upper case
//    //std::string cleanGuess = "";
//    //for (char ch : guess)
//    //{
//    //    if (!isalpha((int)ch))
//    //        return;
//    //    else
//    //        cleanGuess += (char)toupper((int)ch);
//    //}
//
//    if (this->ValidateGuess(guess))
//    {
//        this->guesses.push_back(guess);
//
//        if (guess == this->goalWord)
//        {
//            WinGame();
//        }
//        else if (this->guesses.size() == 6)
//        {
//            LoseGame();
//        }
//    }
//}

void Wordle::WordleGame::PrintGame()
{
    console.ClearConsole();
    for (size_t index = 0; index < 6; ++index)
    {
        if (index < this->guesses.size())
        {
            PrintWordleWord(guesses[index]);
        }
        else
        {
            // Print a blank grid
            PrintWordleBlank();
        }
    }

    PrintWordleKeyboard();
    PrintVerticalLine(48, 2, 30);   // Main divider
    PrintVerticalLine(100, 2, 30);   // Far right divider

    // Print the error message
    console.SetCursorPosition(30, 50);
    console.Print(this->errorMessage);

}

void Wordle::WordleGame::SetPlayerId(Player playerId)
{
    this->playerId = playerId;
}

std::string Wordle::WordleGame::GetGoalWord() const
{
    return this->goalWord;
}

void Wordle::WordleGame::SetGoalWord(std::string goalWord)
{
    this->goalWord = goalWord;
}

//void Wordle::WordleGame::SendGuess(std::string guess)
//{
//    // Will be overridden by derived classes
//}
//
//std::string Wordle::WordleGame::ReceiveGuess()
//{
//    // Will be overridden by derived classes
//    return "";
//}

std::string Wordle::WordleGame::ParseGuessWord(std::string guess, std::string goal)
{
    char guessArray[5];
    char goalArray[5];
    for (size_t index = 0; index < 5; ++index)
    {
        guessArray[index] = guess[index];
        goalArray[index] = goal[index];
    }

    // Blank checks
    for (size_t index = 0; index < 5; ++index)
    {
        if (guessArray[index] == ' ')
        {
            // This is a blank
            guessArray[index] = '%';
        }
    }

    // Green checks
    for (size_t index = 0; index < 5; ++index)
    {
        if (guessArray[index] == goalArray[index])
        {
            // Match
            // Consume in both arrays
            guessArray[index] = '*';
            goalArray[index] = '*';

            // Update
            UpdateKeyboardStatus(guess[index], LetterStatus::Correct);
        }
    }

    // Yellow/Gray checks

    for (size_t guessIndex = 0; guessIndex < 5; ++guessIndex)
    {
        char guessChar = guessArray[guessIndex];

        if (!isalpha(guessChar))
        {
            // Current character is not a letter, meaning it's already been consumed.
            // Just continue the loop.
            continue;
        }

        // Current character is a letter.
        // Check it against the ENTIRE goalArray
        bool foundYellowLetter = false;
        for (size_t goalIndex = 0; goalIndex < 5; ++goalIndex)
        {
            if (guessChar == goalArray[goalIndex])
            {
                // Consume in goal
                // (Guess will be consumed outside loop)
                goalArray[goalIndex] = '?';

                // Set the flag
                foundYellowLetter = true;

                // Stop searching in this direction
                // Since we could consume an extra letter accidentally
                break;
            }
        }

        if (foundYellowLetter)
        {
            // Yellow
            guessArray[guessIndex] = '?';
            UpdateKeyboardStatus(guess[guessIndex], LetterStatus::WrongPlace);
        }
        else
        {
            // Gray
            guessArray[guessIndex] = '#';
            UpdateKeyboardStatus(guess[guessIndex], LetterStatus::Incorrect);
        }
    }

    std::string rv = "";

    for (size_t index = 0; index < 5; ++index)
    {
        rv += guessArray[index];
    }

    return rv;
}

void Wordle::WordleGame::UpdateKeyboardStatus(char letter, LetterStatus newStatus)
{
    size_t index = (size_t)(letter - 'A');
    LetterStatus currentStatus = this->keyboardStatuses[index];
    //std::cout << "DEBUG: Setting letter=" << letter << " [index=" << index << "] to "
    //    << (int)newStatus << " from " << (int)currentStatus << "\n";
    if (newStatus == LetterStatus::Correct)
    {
        // Always set to Correct
        this->keyboardStatuses[index] = newStatus;
    }
    else if (newStatus == LetterStatus::WrongPlace)
    {
        // Set to WrongPlace, unless it's already Correct
        if (currentStatus != LetterStatus::Correct)
        {
            this->keyboardStatuses[index] = newStatus;
        }
    }
    else if (newStatus == LetterStatus::Incorrect)
    {
        // Always set incorrect, unless it's already Correct or WrongPlace
        // (This is actually possible in edge cases)
        if (currentStatus != LetterStatus::Correct &&
            currentStatus != LetterStatus::WrongPlace)
        {
            this->keyboardStatuses[index] = newStatus;
        }
    }
}

Wordle::LetterStatus Wordle::WordleGame::GetKeyboardStatus(char letter) const
{
    size_t index = (size_t)(letter - 'A');
    return this->keyboardStatuses[index];
}

void Wordle::WordleGame::Run()
{
    if (this->playerId == Wordle::Player::NotSet)
    {
        // Single-player mode
        while (!this->isGameOver)
        {
            std::string guess = this->GetUserGuess();
            this->MakeGuess(guess);
        }
    }
    else
    {
        // Multi-player mode

        // Connection loop
        while (!this->isConnectionEstablished)
        {
            this->EstablishConnection();
        }

        // Main game loop
        while (!this->isGameOver)
        {
            // Determine current player
            Wordle::Player currentPlayer = (Wordle::Player)(this->guesses.size() % 2);

            if (this->playerId == currentPlayer)
            {
                this->PrintGame();

                // My turn
                std::string guess = this->GetUserGuess();
                this->MakeGuess(guess);

                this->SendGuess(guess);
            }
            else
            {
                // Opponent's turn
                this->errorMessage = "Waiting for opponent's guess . . . ";
                this->PrintGame();

                std::string guess = this->ReceiveGuess();
                this->errorMessage = "";
                this->MakeGuess(guess);
            }
        }
    }
}

void Wordle::WordleGame::PrintWordleWord(std::string word)
{
    // Determine letter statuses
    std::vector<LetterStatus> letterStatuses = {};

    std::string formatString = ParseGuessWord(word, this->goalWord);

    for (size_t charIndex = 0; charIndex < formatString.size(); ++charIndex)
    {
        if (formatString[charIndex] == '*')
            letterStatuses.push_back(LetterStatus::Correct);
        else if (formatString[charIndex] == '?')
            letterStatuses.push_back(LetterStatus::WrongPlace);
        else if (formatString[charIndex] == '#')
            letterStatuses.push_back(LetterStatus::Incorrect);
        else if (formatString[charIndex] == '%')
            letterStatuses.push_back(LetterStatus::Blank);
        else
            letterStatuses.push_back(LetterStatus::Blank);
    }

    // Print the letters
    console.Print("\n");
    for (int row = 0; row < 3; ++row)
    {
        for (size_t i = 0; i < word.size(); ++i)
        {

            std::string blankString = std::string("       ");
            std::string letterString = std::string("   ") + std::string(1, word[i]) + std::string("   ");
            LetterStatus status = letterStatuses[i];

            uint32_t foreground = 0;
            uint32_t background = 0;

            switch (status)
            {
            case LetterStatus::Correct:
                foreground = WordleGame::WHITE;
                background = WordleGame::GREEN;
                break;
            case LetterStatus::WrongPlace:
                foreground = WordleGame::WHITE;
                background = WordleGame::YELLOW;
                break;
            case LetterStatus::Incorrect:
                foreground = WordleGame::WHITE;
                background = WordleGame::GRAY;
                break;
            case LetterStatus::Blank:
                foreground = WordleGame::DARK_GRAY;
                background = WordleGame::DARK_GRAY;
                break;
            default:
                break;
            }

            //this->Print("  ");
            wprintf(L"  ");
            if (row == 1)
                console.Print(letterString, foreground, background);
            else
                console.Print(blankString, foreground, background);
        }
        console.Print("\n");
    }
}



void Wordle::WordleGame::PrintWordleBlank()
{
    this->PrintWordleWord("     ");
}

void Wordle::WordleGame::PrintWordleKeyboard()
{
    std::vector<std::string> rows =
    {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };

    std::wstring lineSpacerString = L"  ";

    wprintf(L"\n");
    for (size_t index = 0; index < 3; ++index)
    {
        wprintf(L"   ");
        std::string row = rows[index];
        for (int i = 0; i < index; ++i)
            wprintf(lineSpacerString.c_str());

        //wprintf(L"|");

        for (char ch : row)
        {
            auto status = GetKeyboardStatus(ch);
            wprintf(L" ");
            std::string currentLetter = std::string(1, ch);
            uint32_t backgroundColor;
            if (status == LetterStatus::Correct)
                backgroundColor = GREEN;
            else if (status == LetterStatus::WrongPlace)
                backgroundColor = YELLOW;
            else if (status == LetterStatus::Incorrect)
                backgroundColor = DARK_GRAY;
            else
                backgroundColor = GRAY;

            std::string letterString = std::string(" ") + std::string(1, ch) + std::string(" ");
            console.Print(letterString, 0x00FFFFFF, backgroundColor);
        }
        wprintf(L"\n\n");
    }
}

void Wordle::WordleGame::PrintVerticalLine(int column, int startRow, int endRow)
{
    for (int row = startRow; row <= endRow; ++row)
    {
        console.SetCursorPosition(row, column);
        console.Print("|");
    }

}

std::string Wordle::WordleGame::GetUserGuess()
{
    std::string word = "";
    do
    {
        // Output
        PrintGame();
        console.SetCursorPosition(15, 60);
        console.Print("ENTER YOUR GUESS:");

        console.SetCursorPosition(18, 50);
        console.Print("+-------------+");

        console.SetCursorPosition(19, 50);
        console.Print("|             |");

        console.SetCursorPosition(20, 50);
        console.Print("|             |");

        console.SetCursorPosition(21, 50);
        console.Print("|             |");

        console.SetCursorPosition(22, 50);
        console.Print("+-------------+");

        console.SetCursorPosition(20, 55);

        //std::string word;
        std::cin >> word;

        //GuessWord(word);
    } while (!this->ValidateGuess(word));
    return word;
}

void Wordle::WordleGame::MakeGuess(std::string guess)
{
    this->guesses.push_back(guess);

    if (guess == this->goalWord)
    {
        WinGame();
    }
    else if (this->guesses.size() == 6)
    {
        LoseGame();
    }
}

void Wordle::WordleGame::WinGame()
{
    this->PrintGame();

    console.SetCursorPosition(20, 50);
    if ((Wordle::Player)(this->guesses.size() % 2) == this->playerId)
    {
        console.Print("YOU LOSE!!");
    }
    else
    {
        console.Print("YOU WIN!!");
    }
    console.SetCursorPosition(40, 1);

    this->isGameOver = true;
}

void Wordle::WordleGame::LoseGame()
{
    this->PrintGame();

    console.SetCursorPosition(20, 50);
    console.Print("Game over!! No one wins!!");
    console.SetCursorPosition(21, 50);
    console.Print("The word was: " + this->goalWord);

    console.SetCursorPosition(40, 1);

    this->isGameOver = true;
}

bool Wordle::WordleGame::ValidateGuess(std::string& inOutGuess)
{
    // Validate the guess input

    // 5 letter words only
    if (inOutGuess.size() != 5)
    {
        this->errorMessage = "Guess must be 5 letters.";
        return false;
    }
    // Make sure all chars are letters, and convert to upper case
    std::string cleanGuess = "";
    for (char ch : inOutGuess)
    {
        if (!isalpha((int)ch))
        {
            this->errorMessage = "Guess must only have letters.";
            return false;
        }
        else
            cleanGuess += (char)toupper((int)ch);
    }

    // Make sure guess is in valid guess list list
    if (std::find(
        this->legalGuesses.begin(),
        this->legalGuesses.end(),
        cleanGuess) == this->legalGuesses.end()
        )
    {
        this->errorMessage = "\"" + cleanGuess + "\" not in valid guess list.";
        return false;
    }

    this->errorMessage = "";
    inOutGuess = cleanGuess;
    return true;
}
