#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileManagementTest
{
	TEST_CLASS(FileManagementTest)
	{
	public:

		TEST_METHOD(TestRead)
		{
			std::string file_name = "test.txt";
			std::vector<std::string> content{
				"Hello World\n",
				"Line 2\n",
				"\n",
				"Bye!\n"
			};

			std::string expected = "";
			for (auto& line : content)
				expected += line;

			file.open(file_name.c_str(), std::ios::out | std::ios::binary);
			if (!file.is_open()) {
				Assert::Fail(L"Couldn't open the file (fstream)");
			}

			file.write(expected.c_str(), expected.length());
			file.close();

			std::string actual;
			file_.Open(file_name, msh::FileMode::kRead);
			if (!file_.IsOpen()) {
				Assert::Fail(L"Couldn't open the file (FileMangement)");
			}

			auto result = file_.Read();
			if (result.size() != 1)
				Assert::Fail((L"result size: " + std::to_wstring(result.size())).c_str());

			actual = result[0];
			file_.Close();

			std::remove(file_name.c_str());
			
			if (expected.length() != actual.length()) {

				std::wstring expectedwstring = L"";
				for (size_t i = 0; i < expected.length(); i++)
				{
					expectedwstring += expected[i];
				}
				std::wstring actualwstring = L"";
				for (size_t i = 0; i < actual.length(); i++)
				{
					actualwstring += actual[i];
				}

				Assert::Fail((L"expected size: " + std::to_wstring(expected.length()) + L"actual size: " + std::to_wstring(actual.length())
					+ L"\nexpected: *" + expectedwstring + L"*\nactual: *" + actualwstring + L"*").c_str());
			}
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestReadAsync)
		{
			std::string file_name = "test.txt";
			std::vector<std::string> content{
				"Hello World\n",
				"Line 2\n",
				"\n",
				"Bye!\n"
			};

			std::string expected = "";
			for (auto& line : content)
				expected += line;

			file.open(file_name, std::ios::out | std::ios::binary);
			if (!file.is_open()) {
				Assert::Fail(L"Couldn't open the file (fstream)");
			}

			file.write(expected.c_str(), expected.length());
			file.close();

			std::string actual;
			file_.Open(file_name, msh::FileMode::kRead);
			if (!file_.IsOpen()) {
				Assert::Fail(L"Couldn't open the file (FileMangement)");
			}

			msh::Flow buffer;
			std::optional<std::string> result;
			std::thread t1([this, &buffer]() {
				file_.ReadAsync(buffer);
			});
			std::thread t2([&buffer, &result]() {
				result = buffer.pop_async();
			});

			t1.join();
			t2.join();
			if (!result.has_value())
				Assert::Fail(L"result is empty!");

			actual = result.value();
			file_.Close();

			std::remove(file_name.c_str());

			if (expected.length() != actual.length()) {
				Assert::Fail((L"expected size: " + std::to_wstring(expected.length()) + L"actual size: " + std::to_wstring(actual.length())).c_str());
			}
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestWrite)
		{
			std::string file_name = "test.txt";
			std::vector<std::string> content{
				"Hello World\n",
				"Line 2\n",
				"\n",
				"Bye!\n"
			};

			std::string expected = "";
			for (auto& line : content)
				expected += line;

			file_.Open(file_name, msh::FileMode::kWrite);
			if (!file_.IsOpen()) {
				Assert::Fail(L"Couldn't open the file (FileMangement)");
			}

			file_.Write(expected);
			file_.Close();

			file.open(file_name, std::ios::in | std::ios::binary);
			if (!file.is_open())
				Assert::Fail(L"Couldn't open the file (fstream)");
			
			std::string actual;
			actual.resize(expected.length());
			file.read(&actual[0], actual.length());
			file.close();

			std::remove(file_name.c_str());

			if (expected.length() != actual.length()) {
				Assert::Fail((L"expected size: " + std::to_wstring(expected.length()) + L"actual size: " + std::to_wstring(actual.length())).c_str());
			}
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(TestWriteAsync)
		{
			std::string file_name = "test.txt";
			std::vector<std::string> content{
				"Hello World\n",
				"Line 2\n",
				"\n",
				"Bye!\n"
			};

			std::string expected = "";
			for (auto& line : content)
				expected += line;

			file_.Open(file_name, msh::FileMode::kWrite);
			if (!file_.IsOpen()) {
				Assert::Fail(L"Couldn't open the file (FileMangement)");
			}

			msh::Flow buffer;
			buffer.push_async(expected);
			buffer.finished();
			file_.WriteAsync(buffer);
			file_.Close();

			file.open(file_name, std::ios::in | std::ios::binary);
			if (!file.is_open())
				Assert::Fail(L"Couldn't open the file (fstream)");

			std::string actual;
			actual.resize(expected.length());
			file.read(&actual[0], actual.length());
			file.close();

			std::remove(file_name.c_str());

			if (expected.length() != actual.length()) {
				Assert::Fail((L"expected size: " + std::to_wstring(expected.length()) + L"actual size: " + std::to_wstring(actual.length())).c_str());
			}
			Assert::AreEqual(expected, actual);
		}

	private:
		std::fstream file;
		msh::FileManagement file_;
	};
}
