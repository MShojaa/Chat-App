#include "pch.h"
#include "MainScreen.h"

int MainScreen::SelectMenu() {

	while (true) {
		presenter_.ClearScreen();
		selected_menu_ = presenter_.SelectMenu(menu_, selected_menu_);
		switch (selected_menu_) {
		case mshEvent::EnumEvent::kListen:
		{
			int port = presenter_.GetPort(54000);
			if (port > -1)
				view_model_.Listen(port);
			break;
		}
		case mshEvent::EnumEvent::kConnect:
		{
			std::string ip = presenter_.GetIp();
			int port = presenter_.GetPort(54000);
			view_model_.Connect(ip, port);
			break;
		}
		default:
			break;
		}
	}

	return selected_menu_;
}

MainScreen::MainScreen(const std::vector<std::string>& menu, msh::ViewModel& view_model, msh::Presenter &presenter) :
	selected_menu_(0), menu_(menu), view_model_(view_model), presenter_(presenter) {}