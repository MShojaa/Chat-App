#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SocketLibTest
{
	TEST_CLASS(SocketLibTest)
	{
	public:
		
		TEST_METHOD(SendReceiveTest)
		{
			OpenConnection();

			// Sending and receiving message
			std::string expected = "Hello World";
			server_.Send(expected);
			std::optional<std::string> actual = client_.Receive();
			if (!actual.has_value()) {
				std::wstring message = L"Client couldn't receive the message";
				Assert::Fail(message.c_str());
			}

			if (expected.length() != actual.value().length()) {
				std::wstring message = L"expected size: " + std::to_wstring(expected.length()) +
					L"actual size: " + std::to_wstring(actual.value().length());
				Assert::Fail(message.c_str());
			}

			CloseConnection();

			Assert::AreEqual(expected, actual.value());
		}

		TEST_METHOD(SendReceiveAsyncTest)
		{
			OpenConnection();

			// Sending and receiving message
			std::string expected = "Hello World";

			// Sends message
			msh::Flow sending_buffer;
			std::thread write_buffer_thread([&] {
				sending_buffer.push_async(expected);  // Writes data to buffer
				sending_buffer.finished();
				});
			std::thread sending_thread([&] {
				server_.SendAsync(sending_buffer);    // Consumes data from buffer
				});

			// Receives message
			msh::Flow receiving_buffer;
			std::thread receiving_thread([&] {
				client_.ReceiveAsync(receiving_buffer);      // Writes data to buffer
				});
			std::optional<std::string> actual;
			std::thread read_buffer_thread([&] {
				actual = receiving_buffer.pop_async();  // Consumes data from buffer
				});

			write_buffer_thread.join();
			sending_thread.join();
			receiving_thread.join();
			read_buffer_thread.join();

			if (!actual.has_value()) {
				std::wstring message = L"Client couldn't receive the message";
				Assert::Fail(message.c_str());
			}

			if (expected.length() != actual.value().length()) {
				std::wstring message = L"expected size: " + std::to_wstring(expected.length()) +
					L"actual size: " + std::to_wstring(actual.value().length());
				Assert::Fail(message.c_str());
			}

			CloseConnection();

			Assert::AreEqual(expected, actual.value());
		}

		TEST_METHOD(SendReceiveAsyncUntilTest)
		{
			OpenConnection();

			// Sending and receiving message
			std::string expected = "Hello World";
			std::string last_packet = "$$end$$";

			// Sends message
			msh::Flow sending_buffer;
			std::thread write_buffer_thread([&] {
				sending_buffer.push_async(expected);  // Writes data to buffer
				Sleep(1000);
				sending_buffer.push_async(last_packet);        // Writes the last packet for receiver to buffer
				sending_buffer.finished();
				});
			std::thread sending_thread([&] {
				server_.SendAsync(sending_buffer);    // Consumes data from buffer
				});

			// Receives message
			msh::Flow receiving_buffer;
			std::thread receiving_thread([&] {
				client_.ReceiveAsyncUntil(receiving_buffer, last_packet);  // Writes data to buffer
				});
			std::optional<std::string> actual;
			std::thread read_buffer_thread([&] {
				actual = receiving_buffer.pop_async();  // Consumes data from buffer
				});

			write_buffer_thread.join();
			sending_thread.join();
			receiving_thread.join();
			read_buffer_thread.join();

			if (!actual.has_value()) {
				std::wstring message = L"Client couldn't receive the message";
				Assert::Fail(message.c_str());
			}

			if (expected.length() != actual.value().length()) {
				std::wstring message = L"expected size: " + std::to_wstring(expected.length()) +
					L"actual size: " + std::to_wstring(actual.value().length());
				Assert::Fail(message.c_str());
			}

			CloseConnection();

			Assert::AreEqual(expected, actual.value());
		}

	private:
		msh::SocketLib server_;
		msh::SocketLib client_;

		void OpenConnection() {
			// Listen
			int port = 54000;
			std::thread listening_thread([&] {
				if (!server_.Listen(port)) {
					std::wstring message = L"Couldn't listen on the port: " + std::to_wstring(port);
					Assert::Fail(message.c_str());
				}
				});

			// Connect
			std::string ip = server_.GetLocalIPV4();
			bool result;
			for (size_t i = 0; i < 3; ++i) {
				result = client_.Connect(ip, port);
				if (result)
					break;
			}
			if (!result) {
				std::wstring ip_wstring(ip.cbegin(), ip.cend());
				std::wstring message = L"Couldn't connect to " + ip_wstring + L":" + std::to_wstring(port);
				Assert::Fail(message.c_str());
			}
			listening_thread.join();
		}

		void CloseConnection() {
			server_.CloseSocket();
			client_.CloseSocket();
		}
	};
}
