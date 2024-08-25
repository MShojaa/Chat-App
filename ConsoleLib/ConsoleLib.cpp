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

    COORD ConsoleLib::GetCursorPosition() {
        static CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(handle_, &csbi);
        return csbi.dwCursorPosition;
    }

    COORD ConsoleLib::GetConsoleWindowSize() {
        static CONSOLE_SCREEN_BUFFER_INFO csbi;

        COORD coord{};
        if (GetConsoleScreenBufferInfo(handle_, &csbi)) {
            coord.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            coord.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }

        return coord;
    }

    void ConsoleLib::CursorGotoX(const int x) {
        COORD cursor_position = GetCursorPosition();
        if (x > -1)
            SetCursorPosition(x, cursor_position.Y);
        else
            SetCursorPosition(0, cursor_position.Y);
    }

    void ConsoleLib::CursorGoRight(const int offset) {
        COORD cursor_position = GetCursorPosition();

        COORD console_size = GetConsoleWindowSize();

        int cursor_position_X = cursor_position.X + offset;
        if (cursor_position_X < console_size.X)
            SetCursorPosition(cursor_position_X, cursor_position.Y);
        else
            SetCursorPosition(console_size.X - 1, cursor_position.Y);
    }

    void ConsoleLib::CursorGoLeft(const int offset) {
        COORD cursor_position = GetCursorPosition();

        int cursor_position_X = cursor_position.X - offset;
        if (cursor_position_X > -1)
            SetCursorPosition(cursor_position_X, cursor_position.Y);
        else
            SetCursorPosition(0, cursor_position.Y);
    }

    void ConsoleLib::ClearScreen(ClearScreenMode mode) const {
        switch (mode)
        {
        case ClearScreenMode::kAfterChar:
            Print("\033[0J");
            return;
        case ClearScreenMode::kBeforeChar:
            Print("\033[1J");
            return;
        case ClearScreenMode::kScreen:
            Print("\033[2J");
            return;
        default:
            return;
        }
    }

    void ConsoleLib::ClearLine(ClearLineMode mode) const {
        switch (mode)
        {
        case ClearLineMode::kAfterChar:
            Print("\033[0K");
            return;
        case ClearLineMode::kBeforeChar:
            Print("\033[1K");
            return;
        case ClearLineMode::kLine:
            Print("\033[2K");
            return;
        default:
            return;
        }
    }

    void ConsoleLib::Print(const int number) {
        PRINT(number);
    }

    void ConsoleLib::Print(const char ch) {
        PRINT(ch);
    }

    void ConsoleLib::Print(const std::string& str) {
        PRINT(str);
    }

    void ConsoleLib::Print(const std::string& str, const size_t start_index) {
        for (size_t i = start_index; i < str.length(); ++i) {
            Print(str[i]);
        }
    }

    std::string ConsoleLib::GetLine(const int max_length) {
        std::optional<std::string> line = GetLine(max_length, [](const char) {return true; });
        if (line.has_value())
            return std::string();
        else
            return line.value();
    }

    std::optional<std::string> ConsoleLib::GetLine(const int max_length, bool(*is_valid)(const char key)) {
        char key;
        std::string line = "";
        int x_pos = 0;

        while (true) {

            key = _getch();

            switch (key)
            {
            case kDoubleKey:

                key = _getch();
                switch (key)
                {
                case kArrowLeft:
                    if (x_pos > 0) {
                        CursorGoLeft(1);
                        --x_pos;
                    }
                    break;
                case kArrowRight:
                    if (x_pos < max_length - 1) {
                        CursorGoRight(1);
                        ++x_pos;
                    }
                    break;
                default:
                    break;
                }
                break;
            case kBackspace:
                CursorGoLeft(1);
                Print(line, x_pos);
                Print(' ');
                --x_pos;
                line.erase(line.cbegin() + x_pos);
                CursorGotoX(x_pos);
                break;
            case kEnter:
                return line;
            case kEsc:
                return std::nullopt;
            default:
                if (line.length() < max_length)
                    if (is_valid(key)) {
                        Print(key);
                        line += key;
                        ++x_pos;
                    }
                break;
            }
        }
    }

    std::optional<std::string> ConsoleLib::GetLine(const std::string& default_line, const int max_length, bool(*is_valid)(const char key)) {
        char key;
        std::string line = default_line;
        ShowCursor(false);
        Print(line);
        CursorGoLeft((int)line.length());
        ShowCursor(true);
        int x_pos = 0;

        bool has_default_value = !line.empty();

        while (true) {

            key = _getch();

            switch (key)
            {
            case kDoubleKey:

                key = _getch();
                switch (key)
                {
                case kArrowLeft:
                    if (x_pos > 0) {
                        CursorGoLeft(1);
                        --x_pos;
                    }
                    break;
                case kArrowRight:
                    if (x_pos < max_length - 1) {
                        CursorGoRight(1);
                        ++x_pos;
                    }
                    break;
                default:
                    break;
                }
                break;
            case kBackspace:
                CursorGoLeft(1);
                Print(line, x_pos);
                Print(' ');
                --x_pos;
                line.erase(line.cbegin() + x_pos);
                CursorGotoX(x_pos);
                break;
            case kEnter:
                return line;
            case kEsc:
                return std::nullopt;
            default:
                if (line.length() < max_length)
                    if (is_valid(key)) {
                        Print(key);
                        line += key;
                        ++x_pos;
                    }
                break;
            }
        }
    }

    std::optional<std::string> ConsoleLib::GetLine(const int max_length, bool(*is_valid)(const char key, const std::string& line)) {
        char key;
        std::string line = "";
        int x_pos = 0;

        while (true) {

            key = _getch();

            switch (key)
            {
            case kDoubleKey:

                key = _getch();
                switch (key)
                {
                case kArrowLeft:
                    if (x_pos > 0) {
                        CursorGoLeft(1);
                        --x_pos;
                    }
                    break;
                case kArrowRight:
                    if (x_pos < max_length - 1) {
                        CursorGoRight(1);
                        ++x_pos;
                    }
                    break;
                default:
                    break;
                }
                break;
            case kBackspace:
                CursorGoLeft(1);
                Print(line, x_pos);
                Print(' ');
                --x_pos;
                line.erase(line.cbegin() + x_pos);
                CursorGotoX(x_pos);
                break;
            case kEnter:
                return line;
            case kEsc:
                return std::nullopt;
            default:
                if (line.length() < max_length)
                    if (is_valid(key, line)) {
                    Print(key);
                    line += key;
                    ++x_pos;
                    }
                break;
            }
        }
    }

    inline char ConsoleLib::_getch() {
        return ::_getch();
    }

    int ConsoleLib::_getint(const int max_digit_length) {
        std::optional<std::string> number = GetLine(max_digit_length, [](const char ch, const std::string &number) {
            return ((ch >= '0' && ch <= '9') ||
                (number.empty() && ch == '-'));
            });

        if (number.has_value()) {
            return std::stoi(number.value());
        } else {
            return (int)INFINITY;
        }
    }

    int ConsoleLib::_getuint(const int max_digit_length) {
        std::optional<std::string> number = GetLine(max_digit_length, [](const char ch) {
            return (ch >= '0' && ch <= '9');
            });

        if (number.has_value()) {
            return std::stoi(number.value());
        } else {
            return -1;
        }
    }

    ConsoleLib::ConsoleLib() {
        // Gets the console handle for standard output
        handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
    }
}