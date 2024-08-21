#pragma once

#include "pch.h"

namespace msh {
    enum class ClearLineMode {
        kAfterChar, kBeforeChar, kLine
    };
    enum class ClearScreenMode {
        kAfterChar, kBeforeChar, kScreen
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
        /// Gets character from console and doesn't show it on the console
        /// </summary>
        /// <returns> character from keyboard input </returns>
        static char _getch();

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