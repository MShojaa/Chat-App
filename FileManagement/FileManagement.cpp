#include "pch.h"
#include "framework.h"

#include "FileManagement.h"

namespace msh {
	FileManagement::FileManagement() {
		hFile_ = INVALID_HANDLE_VALUE;
		mode_ = FileMode::kInvalid;
	}

	FileManagement::FileManagement(const std::string& path, FileMode mode) {
		Open(path, mode);
	}

	void FileManagement::Open(const std::string& path, FileMode mode) {

		mode_ = mode;
		
		int open_mode;
		unsigned long read_or_write;
		switch (mode)
		{
		case FileMode::kRead:
			open_mode = OPEN_EXISTING;
			read_or_write = GENERIC_READ;
			break;
		case FileMode::kWrite:
			open_mode = CREATE_NEW;
			read_or_write = GENERIC_WRITE;
			break;
		default:
			return;
		}

		hFile_ = CreateFileA(
			path.c_str(),                // File name
			read_or_write,               // Open for reading or writing
			0,                           // Do not share
			NULL,                        // Default security
			open_mode,                   // Opens existing file or creates new file
			FILE_ATTRIBUTE_NORMAL,       // Normal file
			NULL                         // No template
		);
	}

	bool FileManagement::IsOpen() {
		//return hFile_.is_open();
		return hFile_ != INVALID_HANDLE_VALUE;
	}

	std::vector<std::string> FileManagement::Read() {

		if (mode_ == FileMode::kWrite)
			return std::vector<std::string>();

		std::vector<std::string> buffer;

		DWORD bytesRead;
		DWORD chunk_size = 1024;
		std::string chunk;
		while (true) {
			// Reads the file into the buffer
			chunk.resize(chunk_size);
			BOOL result = ReadFile(hFile_, chunk.data(), chunk_size, &bytesRead, NULL);
			if (!result) {
				std::cerr << "Error reading file: " << GetLastError() << std::endl;
				//this->Close();
				return std::vector<std::string>();
			}

			// Ensures all bytes were read
			chunk.resize(bytesRead);
			buffer.emplace_back(chunk);
			if (bytesRead != chunk_size) {
				break;
			}
		}

		return buffer;
	}

	void FileManagement::ReadAsync(msh::Flow<std::string>& buffer) {

		if (mode_ == FileMode::kWrite)
			return;

		DWORD bytesRead;
		DWORD chunk_size = 1024;
		std::string chunk;
		while (true) {
			// Reads the file into the buffer
			chunk.resize(chunk_size);
			BOOL result = ReadFile(hFile_, chunk.data(), chunk_size, &bytesRead, NULL);
			if (!result) {
				std::cerr << "Error reading file: " << GetLastError() << std::endl;
				//this->Close();
				return;
			}

			// Ensures all bytes were read
			chunk.resize(bytesRead);
			buffer.push_async(chunk);
			if (bytesRead != chunk_size) {
				buffer.finished();
				break;
			}
		}
	}

	void FileManagement::Write(const std::string& content) {

		if (mode_ == FileMode::kRead)
			return;

		// Writes the data to the file
		DWORD bytesWritten;
		BOOL result = WriteFile(hFile_, content.data(), static_cast<DWORD>(content.length()), &bytesWritten, NULL);
		if (!result || bytesWritten != content.length()) {
			std::cerr << "Error writing to file: " << GetLastError() << std::endl;
			//this->Close();
			return;
		}
	}

	void FileManagement::WriteAsync(msh::Flow<std::string>& buffer) {
		while (true) {
			std::optional<std::string> content = buffer.pop_async();
			if (!content.has_value())
				return;

			// Writes the data to the file
			DWORD bytesWritten;
			BOOL result = WriteFile(hFile_, content.value().data(), static_cast<DWORD>(content.value().length()), &bytesWritten, NULL);
			if (!result || bytesWritten != content.value().length()) {
				std::cerr << "Error writing to file: " << GetLastError() << std::endl;
				//this->Close();
				return;
			}
		}
	}

	void FileManagement::Close() {
		if (hFile_ != INVALID_HANDLE_VALUE) {
			CloseHandle(hFile_);
			hFile_ = INVALID_HANDLE_VALUE;
		}

	}

	long long FileManagement::GetSize() {
		if (hFile_ == INVALID_HANDLE_VALUE)
			return -1;

		LARGE_INTEGER file_size;
		if (GetFileSizeEx(hFile_, &file_size)) {
			return file_size.QuadPart;
		} else {
			std::cout << "file size error: " << GetLastError() << std::endl;
			return -1;
		}
	}

	long long FileManagement::GetSize(const std::string& file_name) {
		this->Open(file_name, msh::FileMode::kRead);
		if (!this->IsOpen())
			return -1;

		long long size = this->GetSize();

		this->Close();

		return size;
	}

	FileManagement::~FileManagement() {
		this->Close();
	}

	size_t FileManagement::SizeOfFile() {
		return GetFileSize(hFile_, NULL);
	}
}