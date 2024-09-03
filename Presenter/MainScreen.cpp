#include "pch.h"
#include "MainScreen.h"

bool MainScreen::SelectMenu() {

	while (true) {
		presenter_.ClearScreen();
		selected_menu_ = presenter_.SelectMenu(menu_, selected_menu_);
		switch (selected_menu_) {
		case mshEvent::EnumEvent::kListen:
		{
			ip_port_.first = view_model_.GetHostIp();
			presenter_.Print(ip_port_.first + ':').GetPort(ip_port_.second);
			if (ip_port_.second > -1) {
				presenter_.ShowCursor(false).ClearScreen().Print("Listening on " + ip_port_.first + ':' + std::to_string(ip_port_.second));
				if (view_model_.Listen(ip_port_.second)) {
					presenter_.Print("\nConnection has been stablished\n").ShowCursor(true);
					return true;
				}
				view_model_.CloseConnection();
				presenter_.Print("Connection failed\n");
				return false;
			}
			break;
		}
		case mshEvent::EnumEvent::kConnect:
		{
			presenter_.GetIpPort(ip_port_).ShowCursor(false).ClearScreen();
			presenter_.Print("Connecting to: ").Print(ip_port_.first + ':' + std::to_string(ip_port_.second));
			for (size_t i = 0; i < 5; ++i) {
				if (view_model_.Connect(ip_port_.first, ip_port_.second)) {
					presenter_.Print("\nConnection has been stablished\n").ShowCursor(true);
					return true;
				}
				view_model_.CloseConnection();
				presenter_.Print("\nReconnecting...");
			}
			return false;
		}
		default:
			break;
		}
	}

	return selected_menu_;
}

MainScreen::MainScreen(const std::vector<std::string>& menu, msh::ViewModel& view_model, msh::Presenter &presenter) :
	selected_menu_(0), menu_(menu), view_model_(view_model), presenter_(presenter) {
	ip_port_ = std::make_pair("10.10.17.254", 54000);
}