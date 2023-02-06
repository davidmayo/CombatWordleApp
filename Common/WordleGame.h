#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <time.h>

#include "ColorConsole.h"

namespace Wordle
{
	enum class LetterStatus
	{
		Correct,
		WrongPlace,
		Incorrect,
		Blank,
		Undetermined
	};

	enum class Player
	{
		Server = 0,
		Client = 1,
		NotSet = -1
	};

	class WordleGame
	{
	public:
		WordleGame();
		WordleGame(std::string goalWord);
		//void GuessWord(std::string guess);
		void PrintGame();
		void SetPlayerId(Player playerId);
		
		std::string GetGoalWord() const;
		void SetGoalWord(std::string goalWord);
		
		// Pure virtual functions
		// Implemented by   WordleGameServer   and   WordleGameClient   classes
		virtual void SendGuess(std::string guess) = 0;
		virtual std::string ReceiveGuess() = 0;
		virtual void EstablishConnection() = 0;

		// Colors in ARGB format
		const static uint32_t GREEN = 0x004A8A44;
		const static uint32_t YELLOW = 0x00A99438;
		const static uint32_t LIGHT_GRAY = 0x00CCCCCC;
		const static uint32_t GRAY = 0x005A5A5C;
		const static uint32_t DARK_GRAY = 0x00222222;
		const static uint32_t WHITE = 0x00FFFFFF;
		const static uint32_t BLACK = 0x00000000;

		/// <summary>
		/// Parse the guess word and return the format string for that word
		/// </summary>
		/// <param name="guess"></param>
		/// <param name="goal"></param>
		/// <returns></returns>
		std::string ParseGuessWord(std::string guess, std::string goal);
		void UpdateKeyboardStatus(char letter, LetterStatus newStatus);
		LetterStatus GetKeyboardStatus(char letter) const;

		void Run();
	protected:
		std::string goalWord;
		std::vector<std::string> guesses;

		std::vector<std::string> correctLetters;
		std::vector<std::string> incorrectLetters;
		std::vector<std::string> wrongPlaceLetters;

		std::vector<std::string> legalGuesses;

		Player playerId;	// Current player ID.

		bool isGameOver;
		bool isConnectionEstablished;
		std::string errorMessage;	// Error message on invalid guess

		ColorConsole::ColorConsole console;

		void Initialize();

		void PrintWordleWord(std::string word);
		void PrintWordleBlank();
		void PrintWordleKeyboard();
		void PrintVerticalLine(int column, int startRow, int endRow);

		LetterStatus keyboardStatuses[26];

		bool ValidateGuess(std::string& inOutGuess);

		//void GetUserGuess();
		std::string GetUserGuess();
		void MakeGuess(std::string guess);
		void WinGame();
		void LoseGame();
	};
}