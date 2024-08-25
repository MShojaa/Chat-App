#pragma once

#include "pch.h"

class MainScreen {
public:
	int SelectMenu();

	MainScreen(const std::vector<std::string>& menu, msh::ViewModel& view_model, msh::Presenter& presenter);

private:
	std::vector<std::string> menu_;
	int selected_menu_;
	msh::Presenter& presenter_;
	msh::ViewModel& view_model_;
};