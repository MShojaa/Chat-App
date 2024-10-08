#pragma once

#include "pch.h"

namespace msh {

	class ViewModel
	{
	public:
		/// <summary>
		/// Handles UI events
		/// </summary>
		/// <param name="event"> events sent by UI </param>
		/// <returns> true on success or false on failure </returns>
		bool OnEvent(mshEvent::UiEvent *event);

		/// <summary>
		/// Listens on a port given (Server)
		/// </summary>
		/// <param name="port"> port number to open and listens on it </param>
		/// <returns> true on success or false on failure </returns>
		bool Listen(const int port);

		/// <summary>
		/// Connects to the Server (Client)
		/// </summary>
		/// <param name="ip"> Server's ip </param>
		/// <param name="port"> Server's port </param>
		/// <returns> true on success or false on failure </returns>
		bool Connect(const std::string& ip, const int port);

		/// <summary>
		/// Closes connection (server/client)
		/// </summary>
		void CloseConnection();

		/// <summary>
		/// Reads and sends file through network asynchronously
		/// </summary>
		/// <param name="path"> path to the file (relative/full) on the local and sends it to the remote </param>
		/// <returns> true on success or false on failure </returns>
		bool SendFile(const std::filesystem::path& path);

		/// <summary>
		/// Receives and writes data from network to a file asynchronously
		/// </summary>
		/// <returns> true on success or false on failure </returns>
		bool ReceiveFile();

		/// <summary>
		/// Sends message to remote system
		/// </summary>
		/// <param name="message"> message to be sent </param>
		/// <returns> true on success or false on failure </returns>
#undef SendMessage
		bool SendMessage(const std::string& message);

		/// <summary>
		/// Receives message from remote system
		/// </summary>
		/// <param name="received_message"> received message from remote system </param>
		/// <returns> true on success or false on failure </returns>
		bool ReceiveMessage(std::string& received_message);

		std::string GetHostIp();

		/// <summary>
		/// Initializes last_packet_
		/// </summary>
		/// <param name="last_packet"> field variable </param>
		ViewModel(const std::string& last_packet = "$$end$$");

	private:
		/// <summary>
		/// Handles file operations (read/write)
		/// </summary>
		FileManagement file_;

		/// <summary>
		/// Handles network operations (listen/connect/send/receive)
		/// </summary>
		SocketLib socket_[4];

		/// <summary>
		/// Stores last packet for sending and receiving files
		/// </summary>
		std::string last_packet_;
	};
}