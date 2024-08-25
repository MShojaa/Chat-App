// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H_CONSOLE_LIB
#define PCH_H_CONSOLE_LIB

// add headers that you want to pre-compile here
#include "framework.h"

#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <optional>
#include <string>

#define LOG(x) std::cout << x << std::endl
#define PRINT(x) std::cout << x

#endif //PCH_H_CONSOLE_LIB
