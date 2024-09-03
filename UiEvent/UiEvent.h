#pragma once

#include "pch.h"


namespace mshEvent {

	enum EnumEvent {
		kInvalid = -1,
		kListen, kConnect,
		kSendMessage, kReceiveMessage,
		kSendFile, kReceiveFile,
	};

	class UiEvent {
	public:
		std::string ip;
		int port;
		std::string file_name;
		std::string message;
		const static EnumEvent event = kInvalid;

	protected:
		UiEvent() {
			port = 54000;
		}
	};

	class SendFile : UiEvent {
	public:
		SendFile(const std::string& file_name) {
			this->file_name = file_name;
		};

		const static EnumEvent event = kSendFile;
	};

	class ReceiveFile : UiEvent {
	public:
		ReceiveFile() = default;

		const static EnumEvent event = kReceiveFile;
	};

#undef SendMessage
	class SendMessage : UiEvent {
	public:
		SendMessage(const std::string& message) {
			this->message = message;
		};

		const static EnumEvent event = kSendMessage;
	};

	class ReceiveMessage : UiEvent {
	public:
		ReceiveMessage() = default;

		const static EnumEvent event = kReceiveMessage;
	};
};