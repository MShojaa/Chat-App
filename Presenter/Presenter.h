#pragma once

#include "pch.h"

namespace msh {
	enum ConnectionAction {
		kListen, kConnect
	};

	enum ChatAction {
		kSendMessage, kReceiveMassage, kSendFile, kReceiveFile
	};

	enum Keyboard {
		kEnter = 13, kEsc = 27, kArrowUp = 72, kArrowDown = 80, kDoubleKey = -32
	};

	class Presenter
	{
	public:
		void ConnectionScreen();
		void ChatScreen();

		Presenter& SetSelectColor(const int color);
		Presenter& SetDefaultColor(const int color);

		int SelectMenu(const std::vector<std::string>& options, int selected_option = 0) const;

		Presenter(const int color = 12);

	private:
		int select_color_;
		int default_color_;

		ConsoleLib console_;

		void PrintMenu(const std::vector<std::string>& options, const int selected_option) const;
	};
}