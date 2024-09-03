#pragma once

#include "pch.h"

namespace msh {
	namespace String {
		std::string Trim(std::string_view str_view, const char trim_char = ' ');

		std::vector<std::string> Split(std::string_view str_view, const char delim = ',', const char trim_char = ' ');
	}
}