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

	int Presenter::GetPort() {
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