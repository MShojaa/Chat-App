#include "Presenter.h"
#include "ViewModel.h"

#define LOGW(x) std::wcout << x << std::endl
#define PRINTW(x) std::wcout << x

std::string SomeFunc(const std::filesystem::path& path) {
	
	if (std::filesystem::is_regular_file(path)) {
		return path.filename().string();
	}
	return std::string();
}

int main() {
	msh::ViewModel view_model;

	std::string path = "C:\\Users\\source\\repos\\Chat\\Chat.sln";
	LOG(SomeFunc(path));


	return EXIT_SUCCESS;
}