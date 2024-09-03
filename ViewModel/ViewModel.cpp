#include "pch.h"
#include "framework.h"

#include "ViewModel.h"

namespace msh {

#define SERVER_APP

	enum EventMode {
#ifdef SERVER_APP
		kSendMessage, kReceiveMessage, kSendFile, kReceiveFile
#else
		kReceiveMessage, kSendMessage, kReceiveFile, kSendFile
#endif // SERVER_APP
	};

	bool ViewModel::OnEvent(mshEvent::UiEvent *event) {
		switch (event->event)
		{
		case mshEvent::kListen:
			return Listen(event->port);
		case mshEvent::kConnect:
			return Connect(event->ip, event->port);
		case mshEvent::kSendFile:
			return SendFile(event->file_name);
		case mshEvent::kReceiveFile:
			return ReceiveFile();
		case mshEvent::kSendMessage:
			return SendMessage(event->message);
		case mshEvent::kReceiveMessage:
			return ReceiveMessage(event->message);
		default:
			return false;
		}
	}

	bool ViewModel::Listen(const int port) {
		bool result[4]{};
		std::thread send_message_thread([&] {
			result[EventMode::kSendMessage] = socket_[EventMode::kSendMessage].Listen(port);
			});
		std::thread receive_message_thread([&] {
			result[EventMode::kReceiveMessage] = socket_[EventMode::kReceiveMessage].Listen(port + 1);
			});
		std::thread send_file_thread([&] {
			result[EventMode::kSendFile] = socket_[EventMode::kSendFile].Listen(port + 2);
			});
		std::thread receive_file_thread([&] {
			result[EventMode::kReceiveFile] = socket_[EventMode::kReceiveFile].Listen(port + 3);
			});

		send_message_thread.join();
		receive_message_thread.join();
		send_file_thread.join();
		receive_file_thread.join();

		return (result[0] && result[1] && result[2] && result[3]);
	}

	bool ViewModel::Connect(const std::string& ip, const int port) {
		bool result[4]{};
		std::thread receive_message_thread([&] {
			result[EventMode::kReceiveMessage] = socket_[EventMode::kReceiveMessage].Connect(ip, port);  // == Server's SendMessage()
			});
		std::thread send_message_thread([&] {
			result[EventMode::kSendMessage] = socket_[EventMode::kSendMessage].Connect(ip, port + 1);  // == Server's ReceiveMessage()
			});
		std::thread receive_file_thread([&] {
			result[EventMode::kReceiveFile] = socket_[EventMode::kReceiveFile].Connect(ip, port + 2);  // == Server's SendFile()
			});
		std::thread send_file_thread([&] {
			result[EventMode::kSendFile] = socket_[EventMode::kSendFile].Connect(ip, port + 3);  // == Server's ReceiveFile()
			});

		send_message_thread.join();
		receive_message_thread.join();
		send_file_thread.join();
		receive_file_thread.join();

		return (result[0] && result[1] && result[2] && result[3]);
	}

	void ViewModel::CloseConnection() {
		for (size_t i = 0; i < std::size(socket_); ++i) {
			socket_[i].CloseSocket();
		}
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
		std::thread reading_thread([this, &buffer] {
			// Writes the file content in to the buffer
			file_.ReadAsync(buffer);
			});

		// Sends data through network and pops data out of buffer asynchronously
		bool result = false;
		std::thread sending_thread([this, &buffer, &result, &path] {
			// Sends the file name and its size -> separated with comma ','
			result = socket_[EventMode::kSendFile].Send(path.filename().string() + ',' + std::to_string(file_.GetSize()));

			// Receives the answer from client == last_packet_
			auto sent_file = socket_[EventMode::kSendFile].Receive();
			if (!sent_file.has_value() || sent_file.value() != last_packet_)
				return;

			// Sends content of the file
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			result = socket_[EventMode::kSendFile].SendAsync(buffer);

			// Sends the last packet
			std::this_thread::sleep_for(std::chrono::seconds(2));
			result = socket_[EventMode::kSendFile].Send(last_packet_);
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
		auto file_name = socket_[EventMode::kReceiveFile].Receive();
		if (!file_name.has_value())
			return result;  // false

		if (!socket_[EventMode::kReceiveFile].Send(last_packet_))
			return result;  // false

		//long long file_size = 
		// Opens a file to write content in it
		file_.Open(file_name.value(), msh::FileMode::kWrite);
		if (!file_.IsOpen()) {
			std::cout << "error: " << GetLastError() << std::endl;
			return result;  // false
		}

		// Creates an async buffer to receive and write data asynchronously
		msh::Flow<std::string> buffer;

		// Receives data from network and writes them to the buffer asynchronously
		std::thread receiving_thread([this, &buffer, &result] {
			result = socket_[EventMode::kReceiveFile].ReceiveAsyncUntil(buffer, last_packet_);
			});

		// Writes data in a file and pops data out of buffer asynchronously
		std::thread writing_thread([this, &buffer] {
			file_.WriteAsync(buffer);
			});

		receiving_thread.join();
		writing_thread.join();
		file_.Close();

		return result;
	}

	bool ViewModel::SendMessage(const std::string& message) {
		return socket_[EventMode::kSendMessage].Send(message);
	}

	bool ViewModel::ReceiveMessage(std::string& received_message) {
		return socket_[EventMode::kReceiveMessage].Receive(received_message);
	}

	std::string ViewModel::GetHostIp() {
		return SocketLib::GetLocalIPV4();
	}

	ViewModel::ViewModel(const std::string& last_packet) {
		last_packet_ = last_packet;
	}
}