#pragma once

#include "pch.h"

namespace msh {
    class SocketLib {
    public:
        /// <summary>
        /// Listens on the local ip and port (Server)
        /// </summary>
        /// <param name="port"> the port that system needs to listen on </param>
        /// <returns> true on success and false on failure </returns>
        bool Listen(const int port);

        /// <summary>
        /// Connects to the server (Client)
        /// </summary>
        /// <param name="ip"> server's ip </param>
        /// <param name="port"> server's port </param>
        /// <returns> true on success and false on failure </returns>
        bool Connect(const std::string& ip, const int port);

        /// <summary>
        /// Closes the socket
        /// </summary>
        void CloseSocket();

        /// <summary>
        /// Sends data through network
        /// </summary>
        /// <param name="message"> message (data) to send </param>
        /// <returns> true on success and false on failure </returns>
        bool Send(const std::string& message);

        /// <summary>
        /// Sends data through network asynchronously
        /// </summary>
        /// <param name="message"> message (data) to send </param>
        /// <returns> true on success and false on failure </returns>
        bool SendAsync(msh::Flow &buffer);

        /// <summary>
        /// Receives data from 
        /// </summary>
        /// <returns> nullptr on failure and data received on success </returns>
        std::optional<std::string> Receive();

        /// <summary>
        /// Receives data from 
        /// </summary>
        /// <param name="received_message"> Received data from network </param>
        /// <returns> true on success and false on failure </returns>
        bool Receive(std::string &received_message);

        /// <summary>
        /// Receives data from network
        /// If last packet is empty, function is done
        /// </summary>
        /// <param name="buffer"> Received data from network </param>
        /// <returns> true on success and false on failure </returns>
        bool ReceiveAsync(msh::Flow& buffer);

        /// <summary>
        /// Receives data from network until receive the finished_message
        /// </summary>
        /// <param name="finished_message"> Determines last packet </param>
        /// <returns> nullptr on failure and data received on success </returns>
        std::optional<std::string> ReceiveUntil(const std::string& finished_message);

        /// <summary>
        /// Receives data from network until receive the finished_message
        /// </summary>
        /// <param name="buffer"> Received data from network </param>
        /// <param name="finished_message"> Determines last packet </param>
        /// <returns> true on success and false on failure </returns>
        bool ReceiveAsyncUntil(msh::Flow& buffer, const std::string& finished_message);

        /// <summary>
        /// Gets local IPV4 (Server)
        /// </summary>
        /// <returns> local IPV4 </returns>
        std::string GetLocalIPV4();

        /// <summary>
        /// Gets client's ip (Server)
        /// </summary>
        /// <returns> client's ip </returns>
        std::string GetClientIp();

        /// <summary>
        /// Gets client's port (Server)
        /// </summary>
        /// <returns> client's port </returns>
        size_t GetClientPort();

        /// <summary>
        /// Initializes fields to their default values
        /// </summary>
        SocketLib();

        /// <summary>
        /// Closes opened handles
        /// </summary>
        ~SocketLib();

    private:
        /// <summary>
        /// Stores server's attributes
        /// </summary>
        SOCKET server_socket_;

        /// <summary>
        /// Stores client's attributes
        /// </summary>
        SOCKET client_socket_;
    };
}