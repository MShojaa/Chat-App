#pragma once

#include "pch.h"

namespace msh {
	enum ConnectionAction {
		kListen, kConnect
	};

	enum ChatAction {
		kSendMessage, kReceiveMassage, kSendFile, kReceiveFile
	};

	class Presenter
	{
	public:

		Presenter& SetSelectColor(const int color);
		Presenter& SetDefaultColor(const int color);

		int SelectMenu(const std::vector<std::string>& options, int selected_option = 0) const;

		void ClearScreen();

		int GetPort();
		std::string GetIp();

		Presenter(const int color = 12);

	private:
		int select_color_;
		int default_color_;

		ConsoleLib console_;

		void PrintMenu(const std::vector<std::string>& options, const int selected_option) const;
	};
}