#pragma once

#include "pch.h"

namespace msh {
	/// <summary>
	/// Determines open mode of a file
	/// </summary>
	enum class FileMode {
		kInvalid, kRead, kWrite
	};

	class FileManagement
	{
	public:
		/// <summary>
		/// Defaul constructor
		/// </summary>
		FileManagement();

		/// <summary>
		/// Opens a file with the file opening mode given (read / write)
		/// Calls the Open() method
		/// </summary>
		/// <param name="path"> Full/Relative path to the file </param>
		/// <param name="mode"> Read/Write </param>
		FileManagement(const std::string& path, FileMode mode);

		/// <summary>
		/// Opens a file with the file opening mode given (read / write)
		/// </summary>
		/// <param name="path"> Full/Relative path to the file </param>
		/// <param name="mode"> Read/Write </param>
		void Open(const std::string& path, FileMode mode);

		/// <summary>
		/// Checks if the file is open or not
		/// </summary>
		/// <returns> true if the file is open and false if it couldn't open the file </returns>
		bool IsOpen();

		/// <summary>
		/// Reads data from the opened file
		/// </summary>
		/// <returns> Content of file </returns>
		std::vector<std::string> Read();

		/// <summary>
		/// Reads data from the opened file asynchronously
		/// </summary>
		/// <param name="buffer"> stores data async in it </param>
		void ReadAsync(msh::Flow& buffer);

		/// <summary>
		/// Writes content in to the opened file
		/// </summary>
		/// <param name="content"> content to write </param>
		void Write(const std::string& content);

		/// <summary>
		/// Writes content in to the opened file asynchronously
		/// </summary>
		/// <param name="buffer"> content to write </param>
		void WriteAsync(msh::Flow& buffer);

		/// <summary>
		/// Closes the opened file
		/// </summary>
		void Close();

		/// <summary>
		/// Closes the opened file
		/// Calls Close() method
		/// </summary>
		~FileManagement();

	private:
		/// <summary>
		/// Stores handle to the opened file
		/// </summary>
		HANDLE hFile_;

		/// <summary>
		/// Stores open mode (read/write)
		/// </summary>
		FileMode mode_;

		/// <summary>
		/// Returns size of the file
		/// </summary>
		/// <returns> size of the file </returns>
		size_t SizeOfFile();
	};
}