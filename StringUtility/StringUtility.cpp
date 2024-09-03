#include "pch.h"
#include "framework.h"

#include "StringUtility.h"

namespace msh {
	namespace String {
		std::string Trim(std::string_view str_view, const char trim_char) {
			while (str_view[0] == trim_char) {
				str_view.remove_prefix(1);
			}

			while (str_view[str_view.length() - 1] == trim_char) {
				str_view.remove_suffix(1);
			}

			return std::string(str_view);
		}

		std::vector<std::string> Split(std::string_view str_view, const char delim, const char trim_char) {
			std::vector<std::string> result;

			size_t index;
			while (true) {
				index = str_view.find(delim);
				if (index > str_view.length())
					break;

				result.emplace_back(Trim(str_view.substr(0, index), trim_char));
				str_view.remove_prefix(index + 1);
			}

			if (str_view.length() > 0) {
				result.emplace_back(Trim(str_view, trim_char));
			}

			return result;
		}
	}
}