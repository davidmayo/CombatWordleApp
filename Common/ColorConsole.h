#pragma once

#include <string>
#include <vector>

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace ColorConsole
{
	enum class LetterStatus
	{
		Correct,
		WrongPlace,
		Incorrect,
		Blank
	};

	class ColorConsole
	{
	public:
		ColorConsole();

		void ClearConsole() const;
		void Print(
			std::string text,
			uint32_t foregroundaRGB = 0x00FFFFFF,
			uint32_t backgroundaRGB = 0x00000000
		) const;
		void PrintWordle(
			std::string text,
			std::vector<LetterStatus> letterStatuses = {}
		) const;
		void PrintWordleKeyboard(
			std::string correctLetters = "",
			std::string wrongPlaceLetters = "",
			std::string incorrectLetters = ""
		) const;
		void SetCursorPosition(
			int row = 1,
			int column = 1
		) const;

	protected:
		HANDLE  hConsole;
		bool isEnabled = false;
	};
}