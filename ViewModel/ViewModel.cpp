#include "pch.h"
#include "framework.h"

#include "ViewModel.h"

namespace msh {
	bool ViewModel::Listen(const int port) {
		return socket_.Listen(port);
	}

	bool ViewModel::Connect(const std::string& ip, const int port) {
		return socket_.Connect(ip, port);
	}

	bool ViewModel::SendFile(const std::string& file_name) {

		file_.Open(file_name, msh::FileMode::kRead);
		if (!file_.IsOpen())
			return false;

		// Creates an async buffer to read and send data asynchronously
		msh::Flow buffer;

		// Reads data from file and writes them to the buffer asynchronously
		std::thread reading_thread([this, &buffer]() {
			file_.ReadAsync(buffer);
		});

		// Sends data through network and pops data out of buffer asynchronously
		bool result = false;
		std::thread sending_thread([this, &buffer, &result]() {
			result = socket_.SendAsync(buffer);
		});

		reading_thread.join();
		file_.Close();

		sending_thread.join();

		return result;
	}

	bool ViewModel::ReceiveFile(const std::string& file_name) {

		file_.Open(file_name, msh::FileMode::kWrite);
		if (!file_.IsOpen())
			return false;

		// Creates an async buffer to read and send data asynchronously
		msh::Flow buffer;

		// Receives data from network and writes them to the buffer asynchronously
		bool result = false;
		std::thread receiving_thread([this, &buffer, &result]() {
			result = socket_.ReceiveAsync(buffer);
		});

		// Writes data in a file and pops data out of buffer asynchronously
		std::thread writing_thread([this, &buffer]() {
			file_.WriteAsync(buffer);
		});

		receiving_thread.join();
		writing_thread.join();

		return result;
	}

	bool ViewModel::SendMessage(std::string_view message) {
		return false;
	}

	bool ViewModel::ReceiveMessage(std::string& received_message) {
		return false;
	}
}