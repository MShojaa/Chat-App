#pragma once

#include "pch.h"

class UiEvent {
	UiEvent(){};
};

class SendFile : UiEvent {
public:
	SendFile(const std::string& file_name);
	std::string file_name;
};

class ReceiveFile : UiEvent {
public:
	ReceiveFile();
	//std::st
};