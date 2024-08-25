#pragma once

#include "pch.h"

namespace msh {
    enum class ClearLineMode {
        kAfterChar, kBeforeChar, kLine
    };
    enum class ClearScreenMode {
        kAfterChar, kBeforeChar, kScreen
    };
    enum Keyboard {
        kEsc = 27,
        kEnter = 13, kBackspace = 8,
        kArrowUp = 72, kArrowDown = 80, kArrowLeft = 75, kArrowRight = 77, kDoubleKey = -32
    };

    class ConsoleLib {
    public:
        /// <summary>
        /// Sets the text color in the console
        /// </summary>
        /// <param name="color"> text color </param>
        void SetTextColor(const int color) const;

        /// <summary>
        /// Shows or hides the console cursor
        /// </summary>
        /// <param name="is_visible"> true to show the cursor and false to hide it </param>
        void ShowCursor(const bool is_visible) const;

        /// <summary>
        /// Sets the cursor position in the console
        /// </summary>
        /// <param name="x"> horizontal axis (x = 0 -> left side of the console window) </param>
        /// <param name="y"> vertical axis  (y = 0 -> upper side of the console window) </param>
        void SetCursorPosition(const int x, const int y) const;

        /// <summary>
        /// Gets current cursor position on the console
        /// </summary>
        /// <returns> X-Y position of the cursor </returns>
        COORD GetCursorPosition();

        /// <summary>
        /// Gets number of columns and rows that can fit in the console (height and length of the window)
        /// </summary>
        /// <returns> Height and Length of the console window </returns>
        COORD GetConsoleWindowSize();

        /// <summary>
        /// Moves the cursor to the x position with the same y as the current position
        /// </summary>
        /// <param name="x"> new x position of the cursor </param>
        void CursorGotoX(const int x = 0);

        /// <summary>
        /// Moves cursor to the right
        /// </summary>
        /// <param name="offset"> Amount of transfering cursor to the right </param>
        void CursorGoRight(const int offset = 1);

        /// <summary>
        /// Moves cursor to the left
        /// </summary>
        /// <param name="offset"> Amount of transfering cursor to the left </param>
        void CursorGoLeft(const int offset = 1);

        /// <summary>
        /// Clears the console screen in three modes:
        /// 1. kAfterChar:  Clears the screen after the current position of the cursor
        /// 2. kBeforeChar: Clears the screen before the current position of the cursor and the character under the cursor
        /// 3. kScreen:     Clears the whole screen (default value)
        /// </summary>
        /// <param name="mode"> determines how to clear the screen </param>
        void ClearScreen(ClearScreenMode mode = ClearScreenMode::kScreen) const;

        /// <summary>
        /// Clears the console screen in three modes:
        /// 1. kAfterChar:  Clears the characters from after the current position of the cursor to the end of the line
        /// 2. kBeforeChar: Clears the characters from the begining of the line to the character under the cursor (includes the character)
        /// 3. kLine:       Clears the whole line (default value)
        /// </summary>
        /// <param name="mode"> determines how to clear the line </param>
        void ClearLine(ClearLineMode mode = ClearLineMode::kLine) const;

        /// <summary>
        /// Prints a number on the console
        /// </summary>
        /// <param name="number"> number to print </param>
        static void Print(const int number);

        /// <summary>
        /// Prints a character on the console
        /// </summary>
        /// <param name="ch"> character to print </param>
        static void Print(const char ch);

        /// <summary>
        /// Prints a string on the console
        /// </summary>
        /// <param name="str"> string to print </param>
        static void Print(const std::string& str);

        /// <summary>
        /// Prints a string from index to the end
        /// </summary>
        /// <param name="str"> string to print </param>
        /// <param name="start_index"> start index to print </param>
        static void Print(const std::string& str, const size_t start_index);

        std::string GetLine(const int max_length);

        /// <summary>
        /// Gets line from user
        /// </summary>
        /// <param name="max_length"> maximum length of line </param>
        /// <param name="is_valid"> function to validate input data </param>
        /// <returns> std::nullopt if Esc pressed and the line if Enter pressed </returns>
        std::optional<std::string> GetLine(const int max_length, bool (*is_valid)(const char key));
        std::optional<std::string> GetLine(const std::string &line, const int max_length, bool (*is_valid)(const char key));
        std::optional<std::string> GetLine(const int max_length, bool (*is_valid)(const char key, const std::string &line));

        /// <summary>
        /// Gets character from console and doesn't show it on the console
        /// </summary>
        /// <returns> character from keyboard input </returns>
        static char _getch();

        /// <summary>
        /// Gets int from console
        /// </summary>
        /// <param name="max_digit_length"> Maximum number of digits that the number should have </param>
        /// <returns> int number </returns>
        int _getint(const int max_digit_length);

        /// <summary>
        /// Gets unsigned int from console
        /// </summary>
        /// <param name="max_digit_length"> Maximum number of digits that the number should have </param>
        /// <returns> int number </returns>
        int _getuint(const int max_digit_length);

        /// <summary>
        /// Initializes handle_ to the console
        /// </summary>
        ConsoleLib();

    private:
        /// <summary>
        /// Stores handle to console
        /// </summary>
        HANDLE handle_;
    };
}