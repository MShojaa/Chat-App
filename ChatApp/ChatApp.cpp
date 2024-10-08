// ChatApp.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

// TODO: This is an example of a library function
int ChatApp()
{
	msh::Presenter presenter;
	msh::ViewModel main_view_model;

	std::vector<std::string> menu{
		"Server",
			"Client"
	};
	MainScreen main_screen(menu, main_view_model, presenter);
	while (!main_screen.SelectMenu()) {
		presenter.ClearScreen();
		presenter.Print("Do you want to continue?");
		int selected_option = presenter.SelectMenu({
			"Yes",
			"No"
			});
		if (selected_option != 0)
			return EXIT_FAILURE;
	}

	ChatScreen chat_screen(main_view_model, presenter);
	(void)chat_screen.Show();

	return EXIT_SUCCESS;
}
