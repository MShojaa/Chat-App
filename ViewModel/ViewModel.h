#pragma once

#include "pch.h"

namespace msh {
	class ViewModel
	{
	public:
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
		/// Reads and sends file through network asynchronously
		/// </summary>
		/// <param name="file_name"> file_name on the local and sends it to the remote </param>
		/// <returns> true on success or false on failure </returns>
		bool SendFile(const std::string& file_name);

		/// <summary>
		/// Receives and writes data from network to a file asynchronously
		/// </summary>
		/// <param name="file_name"> file_name from remote and saves it on the local system </param>
		/// <returns> true on success or false on failure </returns>
		bool ReceiveFile(const std::string& file_name);

		// TODO:
		
		/// <summary>
		/// Sends message to remote system
		/// </summary>
		/// <param name="message"> message to be sent </param>
		/// <returns> true on success or false on failure </returns>
		bool SendMessage(std::string_view message);

		// TODO:

		/// <summary>
		/// Receives message from remote system
		/// </summary>
		/// <param name="received_message"> received message from remote system </param>
		/// <returns> true on success or false on failure </returns>
		bool ReceiveMessage(std::string &received_message);

	private:
		/// <summary>
		/// Handles file operations (read/write)
		/// </summary>
		FileManagement file_;

		/// <summary>
		/// Handles network operations (listen/connect/send/receive)
		/// </summary>
		SocketLib socket_;
	};
}