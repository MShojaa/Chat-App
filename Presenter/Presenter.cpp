#include "pch.h"
#include "framework.h"

#include "Presenter.h"

namespace msh {
	Presenter& Presenter::SetSelectColor(const int color) {
		select_color_ = color;
		return *this;
	}

	Presenter& Presenter::SetDefaultColor(const int color) {
		default_color_ = color;
		return *this;
	}

	int Presenter::SelectMenu(const std::vector<std::string>& options, int selected_option) const {

		console_.ShowCursor(false);
		bool return_form_funtion = false;

		while (!return_form_funtion) {
			PrintMenu(options, selected_option);
			char key = ConsoleLib::_getch();
			switch (key) {
			case kDoubleKey:
				key = ConsoleLib::_getch();
				switch (key) {
				case kArrowDown:
					if (selected_option < options.size() - 1) {
						++selected_option;
					} else {
						selected_option = 0;
					}
					break;
				case kArrowUp:
					if (selected_option > 0) {
						--selected_option;
					} else {
						selected_option = (int)options.size() - 1;
					}
					break;
				default:
					break;
				}
				break;
			case kEnter:
				return_form_funtion = true;
				break;
			case kEsc:
				return_form_funtion = true;
				selected_option = -1;
				break;
			default:
				break;
			}
		}

		console_.SetTextColor(default_color_);
		console_.ShowCursor(true);
		return selected_option;
	}

	void Presenter::ClearScreen() {
		console_.ClearScreen();
	}

	int Presenter::GetPort(int default_port) {
		std::string line = std::to_string(default_port);
		ShowCursor(false);
		console_.Print(line);
		console_.CursorGoLeft((int)line.length());
		ShowCursor(true);
		auto coord = console_.GetCursorPosition();
		int x_pos = 0;
		const int max_length = 5;

		bool has_default_value = !line.empty();

		auto is_valid = [](const char ch) {
			return(ch >= '0' && ch <= '9');
		};

		char key;
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
						console_.CursorGoLeft(1);
						--x_pos;
					}
					break;
				case kArrowRight:
				{
					int max_pos = min(max_length, (int)line.length());
					if (x_pos < max_pos) {
						console_.CursorGoRight(1);
						++x_pos;
					}
				}
					break;
				default:
					break;
				}
				has_default_value = false;
				break;
			case kBackspace:
				if (has_default_value) {
					console_.ShowCursor(false);
					console_.Print(std::string(line.length(), ' '));
					console_.CursorGoLeft(line.length());
					line = "";
					console_.ShowCursor(true);
					has_default_value = false;
				} else if (x_pos > 0) {
					console_.CursorGoLeft(1);
					console_.Print(line, x_pos);
					console_.Print(' ');
					--x_pos;
					line.erase(line.cbegin() + x_pos);
					console_.CursorGotoX(coord.X + x_pos);
				}

				break;
			case kEnter:
				return std::stoi(line);
			case kEsc:
				return -1;
			default:
				if (line.length() < max_length) {
					if (is_valid(key)) {
						console_.Print(key);
						console_.ShowCursor(false);
						console_.Print(line, x_pos);
						console_.ShowCursor(true);
						console_.CursorGoLeft((int)line.length() - x_pos);
						line += key;
						++x_pos;
					}
				} else if (has_default_value) {
					if (is_valid(key)) {
						console_.Print(key);
						console_.ShowCursor(false);
						console_.Print(std::string(line.length(), ' '));
						console_.CursorGoLeft((int)line.length());
						console_.ShowCursor(true);
						line = key;
						has_default_value = false;
						++x_pos;
					}
				}
				break;
			}
		}
		return console_._getuint(5);
	}

	std::string Presenter::GetIp() {
		return std::string();
	}

	Presenter::Presenter(const int color) {
		(void)SetSelectColor(color).SetDefaultColor(7);
	}

	void Presenter::PrintMenu(const std::vector<std::string>& options, const int selected_option) const {
		console_.ClearScreen(ClearScreenMode::kScreen);

		for (size_t i = 0; i < options.size(); ++i) {
			if (i == selected_option)
				console_.SetTextColor(select_color_);
			else
				console_.SetTextColor(default_color_);

			LOG(options[i]);
		}
	}
}