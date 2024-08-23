#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ViewModelTest
{
	TEST_CLASS(ViewModelTest)
	{
	public:

		TEST_METHOD(SendReceiveFileTest)
		{
			OpenConnection();

			std::filesystem::path path = "test\\test.txt";
			std::filesystem::path directory = path;
			std::string expected = "Hello World\nHow do you do?";

			// Creates directory
			std::filesystem::create_directory(directory.remove_filename());
			if (!std::filesystem::exists(directory))
				Assert::Fail((L"Failed to create the directory: " + directory.wstring()).c_str());

			// Creates sample file
			std::fstream file(path, std::ios::out);
			if (!file.is_open())
				Assert::Fail(L"Failed to create the file");

			file.write(expected.c_str(), expected.length());
			file.close();

			// Sends data
			std::thread sending_thread([&] {
				if (!server_view_model_.SendFile(path))
					Assert::Fail(L"Couldn't send the file");
				});

			// Receives data
			std::thread receiving_thread([&] {
				if (!client_view_model_.ReceiveFile())
					Assert::Fail(L"Couldn't receive the file");
				});

			sending_thread.join();
			receiving_thread.join();

			if (!std::filesystem::exists(path.filename()))
				Assert::Fail(L"Didn't received content file");

			file.open(path.filename(), std::ios::in);
			if (!file.is_open())
				Assert::Fail(L"Couldn't create received content file");

			std::string actual(expected.length(), '\0');
			file.read(&actual[0], expected.length());
			file.close();

			Assert::AreEqual(expected, actual);

			// Removes test folder and file
			try {
				if (std::filesystem::exists(directory)) {
					std::uintmax_t removed = std::filesystem::remove_all(directory);
				}
				else {
					Assert::Fail(L"Directory does not exist");
				}
			}
			catch (const std::filesystem::filesystem_error& e) {
				std::string error = e.what();
				Assert::Fail((L"Error: " + std::wstring(error.cbegin(), error.cend())).c_str());
			}
			if (std::filesystem::exists(path.filename()))
				if (!std::filesystem::remove(path.filename().string().c_str()))
					Assert::Fail(L"file does not exist");

			CloseConnection();
		}

		TEST_METHOD(SendReceiveMessageTest)
		{
			OpenConnection();

			std::string expected = "Hello World\nHow do you do?";

			// Sends data
			std::thread sending_thread([&] {
				if (!server_view_model_.SendMessage(expected))
					Assert::Fail(L"Couldn't send the file");
				});

			// Receives data
			std::string actual;
			std::thread receiving_thread([&] {
				if (!client_view_model_.ReceiveMessage(actual))
					Assert::Fail(L"Couldn't receive the file");
				});

			sending_thread.join();
			receiving_thread.join();

			Assert::AreEqual(expected, actual);

			CloseConnection();
		}

	private:
		msh::SocketLib server_;
		msh::ViewModel server_view_model_;
		msh::ViewModel client_view_model_;

		void OpenConnection() {
			// Listen
			int port = 54000;
			std::thread listening_thread([&] {
				if (!server_view_model_.Listen(port)) {
					std::wstring message = L"Couldn't listen on the port: " + std::to_wstring(port);
					Assert::Fail(message.c_str());
				}
				});

			// Connect
			std::string ip = server_.GetLocalIPV4();
			bool result;
			for (size_t i = 0; i < 3; ++i) {
				result = client_view_model_.Connect(ip, port);
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
			server_view_model_.CloseConnection();
			client_view_model_.CloseConnection();
		}
	};
}
