#include "pch.h"
#include "framework.h"

#include "ViewModel.h"

namespace msh {
	void ViewModel::OnEvent(const UiEvent event) {
		/*switch (event)
		{
		case UiEvent::kSendMessage:
			break;
		case UiEvent::kReceiveMessage:
			break;
		case UiEvent::kSendFile:
			break;
		case UiEvent::kReceiveFile:
			break;
		default:
			break;
		}*/
	}

	bool ViewModel::Listen(const int port) {
		return socket_.Listen(port);
	}

	bool ViewModel::Connect(const std::string& ip, const int port) {
		return socket_.Connect(ip, port);
	}

	void ViewModel::CloseConnection() {
		socket_.CloseSocket();
	}

	bool ViewModel::SendFile(const std::filesystem::path& path) {

		// Checks if the file exists or not
		if (!std::filesystem::is_regular_file(path))
			return false;

		// Opens the file and checks it could open it or not
		file_.Open(path.string(), msh::FileMode::kRead);
		if (!file_.IsOpen())
			return false;

		// Creates an async buffer to read and send data asynchronously
		msh::Flow buffer;

		// Reads data from file and writes them to the buffer asynchronously
		std::thread reading_thread([this, &buffer]() {
			// Writes the file content in to the buffer
			file_.ReadAsync(buffer);
			});

		// Sends data through network and pops data out of buffer asynchronously
		bool result = false;
		std::thread sending_thread([this, &buffer, &result, &path] {
			// Sends the file name
			result = socket_.Send(path.filename().string());
			auto sent_file = socket_.Receive();
			if (!sent_file.has_value() || sent_file.value() != last_packet_)
				return;

			// Sends content of the file
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			result = socket_.SendAsync(buffer);

			// Sends the last packet
			std::this_thread::sleep_for(std::chrono::seconds(2));
			result = socket_.Send(last_packet_);
			});

		// Waits for the file to finish reading its content and closes the file
		reading_thread.join();
		file_.Close();

		// Waits for finishing sending data
		sending_thread.join();

		return result;
	}

	bool ViewModel::ReceiveFile() {
		bool result = false;

		// Receives the file name
		auto file_name = socket_.Receive();
		if (!file_name.has_value())
			return result;  // false

		if (!socket_.Send(last_packet_))
			return result;

		// Opens a file to write content in it
		file_.Open(file_name.value(), msh::FileMode::kWrite);
		if (!file_.IsOpen())
			return result;  // false

		// Creates an async buffer to receive and write data asynchronously
		msh::Flow buffer;

		// Receives data from network and writes them to the buffer asynchronously
		std::thread receiving_thread([this, &buffer, &result]() {
			result = socket_.ReceiveAsyncUntil(buffer, last_packet_);
			});

		// Writes data in a file and pops data out of buffer asynchronously
		std::thread writing_thread([this, &buffer]() {
			file_.WriteAsync(buffer);
			});

		receiving_thread.join();
		writing_thread.join();
		file_.Close();

		return result;
	}

	bool ViewModel::SendMessage(const std::string& message) {
		return socket_.Send(message);
	}

	bool ViewModel::ReceiveMessage(std::string& received_message) {
		return socket_.Receive(received_message);
	}

	ViewModel::ViewModel(const std::string& last_packet) {
		last_packet_ = last_packet;
	}
}