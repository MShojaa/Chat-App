#include "pch.h"
#include "framework.h"

#include "SocketLib.h"

namespace msh {
    bool SocketLib::Listen(const int port) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            // Handle error
            return false;
        }

        // Create socket
        server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_ == INVALID_SOCKET) {
            // Handle error
            WSACleanup();
            return false;
        }

        // Bind socket to port
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        if (bind(server_socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            // Handle error
            closesocket(server_socket_);
            WSACleanup();
            return false;
        }

        // Listen for connections
        if (listen(server_socket_, SOMAXCONN) == SOCKET_ERROR) {
            // Handle error
            closesocket(server_socket_);
            WSACleanup();
            return false;
        }

        // Accept a connection
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        client_socket_ = accept(server_socket_, (sockaddr*)&clientAddr, &clientSize);
        if (client_socket_ == INVALID_SOCKET) {
            // Handle error
            closesocket(server_socket_);
            WSACleanup();
            return false;
        }

        return true;
    }

    bool SocketLib::Connect(const std::string& ip, const int port) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            // Handle error
            return false;
        }

        // Create socket
        client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket_ == INVALID_SOCKET) {
            // Handle error
            WSACleanup();
            return false;
        }

        // Connect to server
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
        serverAddr.sin_port = htons(port);
        if (connect(client_socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            // Handle error
            closesocket(client_socket_);
            WSACleanup();
            return false;
        }

        return true;
    }

    bool SocketLib::Send(const std::string& message) {
        // Send message
        int sendResult = send(client_socket_, message.c_str(), (int)message.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            // Handle error
            return false;
        }

        return true;
    }

    bool SocketLib::SendAsync(msh::Flow& buffer) {
        while (true) {
            std::optional<std::string> temp_buffer = buffer.pop_async();
            if (!temp_buffer.has_value())
                break;

            // Returns false on network error
            if (!this->Send(temp_buffer.value()))
                return false;
        }

        return true;
    }

    std::optional<std::string> SocketLib::Receive() {
        // Receive message
        char buffer[1024];
        int receiveResult = recv(client_socket_, buffer, sizeof(buffer), 0);
        if (receiveResult >= 0) {
            return std::string(buffer, receiveResult);
        } else {
            // Handle error
            return std::nullopt;
        }
    }

    bool SocketLib::ReceiveAsync(msh::Flow& buffer) {

        bool result = false;

        while (true) {
            std::optional<std::string> received_data = this->Receive();
            if (!received_data.has_value()) {
                break;
            }

            if (received_data.value().empty()) {
                result = true;
                break;
            }
            buffer.push_async(received_data.value());
        }

        buffer.finished();
        return result;
    }

    std::string SocketLib::GetLocalIPV4() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
            std::cerr << "Error getting hostname.\n";
            return "";
        }

        struct addrinfo hints = { 0 }, * res;
        hints.ai_family = AF_INET; // IPV4
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(hostname, nullptr, &hints, &res) != 0) {
            std::cerr << "Error getting address info.\n";
            return "";
        }

        char ip[INET_ADDRSTRLEN];
        struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)res->ai_addr;
        inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ip, INET_ADDRSTRLEN);

        freeaddrinfo(res);
        return std::string(ip);
    }

    std::string SocketLib::GetClientIp() {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        if (getpeername(client_socket_, (struct sockaddr*)&clientAddr, &addrLen) == SOCKET_ERROR) {
            std::cerr << "Error getting peer name.\n";
            return "";
        }

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
        return std::string(clientIp);
    }

    size_t SocketLib::GetClientPort() {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        if (getpeername(client_socket_, (struct sockaddr*)&clientAddr, &addrLen) == SOCKET_ERROR) {
            std::cerr << "Error getting peer name.\n";
            return 0;
        }

        return ntohs(clientAddr.sin_port);
    }

    SocketLib::SocketLib() {
        server_socket_ = INVALID_SOCKET;
        client_socket_ = INVALID_SOCKET;

        WSADATA wsaData;
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed.\n";
            exit(1);
        }
    }

    SocketLib::~SocketLib() {
        WSACleanup();
    }
}