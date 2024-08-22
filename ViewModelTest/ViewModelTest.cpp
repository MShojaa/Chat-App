#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ViewModelTest
{
	TEST_CLASS(ViewModelTest)
	{
	public:
		
		TEST_METHOD(SendReceiveTest)
		{
			OpenConnection();

			std::string path = "C:\\Users\\mazia\\Desktop\\SocketLib.cpp";
			std::thread sending_thread([&] {
				if (!server_view_model_.SendFile(path))
					Assert::Fail(L"Couldn't send the file");
				});

			std::thread receiving_thread([&] {
				if (!client_view_model_.ReceiveFile())
					Assert::Fail(L"Couldn't receive the file");
				});

			sending_thread.join();
			receiving_thread.join();

			Assert::IsTrue(true);

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
