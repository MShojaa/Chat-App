#include "pch.h"
#include "framework.h"

#include "ConsoleLib.h"

namespace msh {
    void ConsoleLib::SetTextColor(const int color) const {
        SetConsoleTextAttribute(handle_, color);
    }

    void ConsoleLib::ShowCursor(const bool is_visible) const {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(handle_, &cursorInfo);
        cursorInfo.bVisible = is_visible;  // Set the visibility
        SetConsoleCursorInfo(handle_, &cursorInfo);
    }

    void ConsoleLib::SetCursorPosition(const int x, const int y) const {
        COORD coord{
            static_cast<SHORT>(x),
            static_cast<SHORT>(y)
        };
        SetConsoleCursorPosition(handle_, coord);
    }

    void ConsoleLib::ClearScreen(ClearScreenMode mode) const {
        switch (mode)
        {
        case ClearScreenMode::kAfterChar:
            PRINT("\033[0J");
            return;
        case ClearScreenMode::kBeforeChar:
            PRINT("\033[1J");
            return;
        case ClearScreenMode::kScreen:
            PRINT("\033[2J");
            return;
        default:
            return;
        }
    }

    void ConsoleLib::ClearLine(ClearLineMode mode) const {
        switch (mode)
        {
        case ClearLineMode::kAfterChar:
            PRINT("\033[0K");
            return;
        case ClearLineMode::kBeforeChar:
            PRINT("\033[1K");
            return;
        case ClearLineMode::kLine:
            PRINT("\033[2K");
            return;
        default:
            return;
        }
    }

    inline char ConsoleLib::_getch() {
        return ::_getch();
    }

    ConsoleLib::ConsoleLib() {
        // Gets the console handle for standard output
        handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
    }
}