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

	Presenter& Presenter::ClearScreen() {
		console_.ClearScreen();
		return *this;
	}

	Presenter& Presenter::ClearLine() {
		console_.ClearLine();
		console_.CursorGotoX();
		return *this;
	}

	Presenter& Presenter::ShowCursor(const bool is_visible) {
		console_.ShowCursor(is_visible);
		return *this;
	}

	void Presenter::GetChar(msh::Flow<char>& key) {
		key.push_async(console_._getch());
	}

	/*mshEvent::UiEvent Presenter::UiEvent() {
		auto result = this->GetLine([](char key) {
			return (key != msh::kDoubleKey &&
				key != '\\' && key != '/' &&
				key != ':' && key != '*' &&
				key != '?' && key != '\"' &&
				key != '<' && key != '>' &&
				key != '|'
			);
		});

		if (result.has_value())
			return mshEvent::kInvalid;
		else if (result.value() == "Receive File")
			return mshEvent::kSendFile;
		else
			return mshEvent::kSendMessage;
	}*/

	Presenter& Presenter::Print(const std::string& line) {
		console_.Print(line);
		return *this;
	}

	Presenter& Presenter::log(const std::string& line) {
		console_.Print(line + "\n");
		return *this;
	}

	Presenter& Presenter::GetPort(int &default_port) {
		auto is_valid = [](const char ch) {
			return(ch >= '0' && ch <= '9');
		};

		//console_.Print("Port: ");
		auto result = GetLine(std::to_string(default_port), 5, is_valid);
		if (result.has_value()) {
			if (result.value().empty()) {
				default_port = 54000;
			} else {
				default_port = std::stoi(result.value());
			}
		}

		return *this;
	}

	Presenter& Presenter::GetIpPort(std::pair<std::string, int> &ip_port) {
		auto is_valid = [](const char ch) {
			return((ch >= '0' && ch <= '9') || ch == ':' || ch == '.');
			};

		console_.Print("IP:Port -> ");
		const std::string default_ip_port = ip_port.first + ':' + std::to_string(ip_port.second);
		auto result = GetLine(default_ip_port, 21, is_valid);

		if (!result.has_value()) {
			ip_port = std::make_pair("", 0);
			return *this;
		}

		std::string ip;
		int port;

		int port_index = (int)result.value().find_last_of(':');
		if (port_index < result.value().length()) {
			ip = result.value().substr(0, port_index);
			port = std::stoi(result.value().substr((size_t)port_index + 1));
		} else {
			ip = result.value();
			port = 54000;
		}
		ip_port = std::make_pair(ip, port);
		return *this;
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

	std::optional<std::string> Presenter::GetLine(bool (*is_valid)(const char key), const int max_length) {
		auto coord = console_.GetCursorPosition();
		int x_pos = 0;
		std::string line = "";

		char key;
		while (true) {

			key = console_._getch();

			switch (key)
			{
			case kDoubleKey:

				key = console_._getch();
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
				break;
			case kBackspace:
				if (x_pos > 0) {
					console_.CursorGoLeft(1);
					console_.Print(line, x_pos);
					console_.Print(' ');
					--x_pos;
					line.erase(line.cbegin() + x_pos);
					console_.CursorGotoX(coord.X + x_pos);
				}

				break;
			case kCtrlF:
				if (line.empty())
					return "$$File$$";
				break;
			case kEnter:
				console_.Print('\n');
				return line;
			case kEsc:
				return std::nullopt;
			default:
				if (line.length() < max_length) {
					if (is_valid(key)) {
						console_.Print(key);
						console_.ShowCursor(false);
						console_.Print(line, x_pos);
						console_.ShowCursor(true);
						console_.CursorGoLeft((int)line.length() - x_pos);
						line.insert(line.cbegin() + x_pos, key);
						++x_pos;
					}
				}
				break;
			}
		}
	}

	std::optional<std::string> Presenter::GetLine(const std::string& default_line, const int max_length, bool(*is_valid)(const char key)) {
		std::string line = default_line;
		ShowCursor(false);
		console_.Print(line);
		console_.CursorGoLeft((int)line.length());
		ShowCursor(true);
		auto coord = console_.GetCursorPosition();
		int x_pos = 0;

		bool has_default_value = !line.empty();

		char key;
		while (true) {

			key = console_._getch();

			switch (key)
			{
			case kDoubleKey:

				key = console_._getch();
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
					console_.CursorGoLeft((int)line.length());
					line = "";
					console_.ShowCursor(true);
					has_default_value = false;
				}
				else if (x_pos > 0) {
					console_.CursorGoLeft(1);
					console_.Print(line, x_pos);
					console_.Print(' ');
					--x_pos;
					line.erase(line.cbegin() + x_pos);
					console_.CursorGotoX(coord.X + x_pos);
				}

				break;
			case kEnter:
				return line;
			case kEsc:
				return std::nullopt;
			default:
				if (has_default_value) {
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
				} else if (line.length() < max_length) {
					if (is_valid(key)) {
						console_.Print(key);
						console_.ShowCursor(false);
						console_.Print(line, x_pos);
						console_.ShowCursor(true);
						console_.CursorGoLeft((int)line.length() - x_pos);
						line.insert(line.cbegin() + x_pos, key);
						++x_pos;
					}
				}
				break;
			}
		}
	}
}