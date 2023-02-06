#include "ColorConsole.h"

ColorConsole::ColorConsole::ColorConsole()
{
    bool foundProblemEnabling = false;
    // Set output mode to handle virtual terminal sequences
    this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (this->hConsole == INVALID_HANDLE_VALUE)
    {
        DWORD lastErrorCode = GetLastError();
        wprintf(L"Error calling GetStdHandle. Error code: %i\n", lastErrorCode);
        foundProblemEnabling = true;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(this->hConsole, &dwMode))
    {
        DWORD lastErrorCode = GetLastError();
        wprintf(L"Error calling GetConsoleMode. Error code: %i\n", lastErrorCode);
        foundProblemEnabling = true;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(this->hConsole, dwMode))
    {
        DWORD lastErrorCode = GetLastError();
        wprintf(L"Error calling SetConsoleMode. Error code: %i\n", lastErrorCode);
        foundProblemEnabling = true;
    }

    this->isEnabled = !foundProblemEnabling;
}

void ColorConsole::ColorConsole::ClearConsole() const
{
    if (this->isEnabled)
    {
        // Reset foreground and background colors (and other things)
        // Sequence is always "<ESCAPE_CHAR>[0m"
        wprintf(L"\x1b[0m");

        // Clear the screen
        // Sequence is always "<ESCAPE_CHAR>[2J"
        wprintf(L"\x1b[2J");

        // Move cursor to row 1, column 1
        // Sequence is "<ESCAPE_CHAR>[<ROW>;<COLUMN>H"
        wprintf(L"\x1b[1;1H");
    }
    else
    {
        // Best we can do is call cls.exe, which is slow.
        system("cls");
    }
}

void ColorConsole::ColorConsole::Print(
    std::string text,
    uint32_t foregroundARGB,
    uint32_t backgroundARGB) const
{
    std::wstring wide_text = std::wstring(text.begin(), text.end());
    if (!this->isEnabled)
    {
        // If not enabled, just print the text with no formatting
        wprintf(L"%s", wide_text.c_str());
        return;
    }

    uint32_t foregroundAlpha = (foregroundARGB & 0xFF000000) >> 24;    // Ignored by this function
    uint32_t foregroundRed = (foregroundARGB & 0x00FF0000) >> 16;
    uint32_t foregroundGreen = (foregroundARGB & 0x0000FF00) >> 8;
    uint32_t foregroundBlue = (foregroundARGB & 0x000000FF) >> 0;

    uint32_t backgroundAlpha = (backgroundARGB & 0xFF000000) >> 24;   // Ignored by this function
    uint32_t backgroundRed = (backgroundARGB & 0x00FF0000) >> 16;
    uint32_t backgroundGreen = (backgroundARGB & 0x0000FF00) >> 8;
    uint32_t backgroundBlue = (backgroundARGB & 0x000000FF) >> 0;

    // Configure foreground color
    // Sequence is "<ESCAPE_CHAR>[38;2;<RED>;<GREEN>;<BLUE>m"
    wprintf(L"\x1b[38;2;%i;%i;%im", foregroundRed, foregroundGreen, foregroundBlue);

    // Configure background color
    // Sequence is "<ESCAPE_CHAR>[48;2;<RED>;<GREEN>;<BLUE>m"
    wprintf(L"\x1b[48;2;%i;%i;%im", backgroundRed, backgroundGreen, backgroundBlue);

    // Write the text
    wprintf(L"%s", wide_text.c_str());

    // Reset colors (and other things)
    // Sequence is always "<ESCAPE_CHAR>[0m"
    wprintf(L"\x1b[0m");
}

void ColorConsole::ColorConsole::PrintWordle(
    std::string text,
    std::vector<LetterStatus> letterStatuses) const
{
    uint32_t yellow = 0x00C9B458;
    uint32_t green = 0x006AAA64;
    uint32_t gray = 0x005A5A5C;
    uint32_t white = 0x00FFFFFF;
    uint32_t darkGray = 0x00222222;
    this->Print("\n");
    for (int row = 0; row < 3; ++row)
    {
        for (size_t i = 0; i < text.size(); ++i)
        {

            std::string blankString = std::string("       ");
            std::string letterString = std::string("   ") + std::string(1, text[i]) + std::string("   ");
            LetterStatus status = letterStatuses[i];

            uint32_t foreground = 0;
            uint32_t background = 0;

            switch (status)
            {
            case LetterStatus::Correct:
                foreground = white;
                background = green;
                break;
            case LetterStatus::WrongPlace:
                foreground = white;
                background = yellow;
                break;
            case LetterStatus::Incorrect:
                foreground = white;
                background = gray;
                break;
            case LetterStatus::Blank:
                foreground = darkGray;
                background = darkGray;
                break;
            default:
                break;
            }

            //this->Print("  ");
            wprintf(L"  ");
            if (row == 1)
                this->Print(letterString, foreground, background);
            else
                this->Print(blankString, foreground, background);
        }
        this->Print("\n");
    }
}

void ColorConsole::ColorConsole::PrintWordleKeyboard(
    std::string correctLetters,
    std::string wrongPlaceLetters,
    std::string incorrectLetters) const
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

        for (char ch : row)
        {
            wprintf(L" ");

            std::string currentLetter = std::string(1, ch);
            uint32_t backgroundColor;
            if (correctLetters.find(currentLetter) != std::string::npos)
                backgroundColor = 0x006AAA64;
            else if (wrongPlaceLetters.find(currentLetter) != std::string::npos)
                backgroundColor = 0x00C9B458;
            else if (incorrectLetters.find(currentLetter) != std::string::npos)
                backgroundColor = 0x00333333;
            else
                backgroundColor = 0x00666666;

            std::string letterString = std::string(" ") + std::string(1, ch) + std::string(" ");
            this->Print(letterString, 0x00FFFFFF, backgroundColor);
        }
        wprintf(L"\n\n");
    }
}

void ColorConsole::ColorConsole::SetCursorPosition(int row, int column) const
{
    // Sequence is "<ESCAPE_CHAR>[<ROW>;<COLUMN>H"
    wprintf(L"\x1b[%i;%iH", row, column);
}
