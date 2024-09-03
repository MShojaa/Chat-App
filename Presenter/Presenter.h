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

		Presenter& ClearScreen();

		Presenter& ClearLine();

		Presenter& ShowCursor(const bool is_visible);

		void GetChar(msh::Flow<char> &key);

		//void UiEvent();

		Presenter& Print(const std::string& line);

		Presenter& log(const std::string& line);

		Presenter& GetPort(int &default_port);

		Presenter& GetIpPort(std::pair<std::string, int>& ip_port);

		Presenter(const int color = 12);

		std::optional<std::string> GetLine(bool (*is_valid)(const char key), const int max_length = 512);

	private:
		int select_color_;
		int default_color_;

		ConsoleLib console_;

		void PrintMenu(const std::vector<std::string>& options, const int selected_option) const;

		std::optional<std::string> GetLine(const std::string &line, const int max_length, bool (*is_valid)(const char key));
	};
}