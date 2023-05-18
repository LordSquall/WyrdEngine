#pragma once
#include <filesystem>

/* core wyrd includes */

/* local includes */


using namespace Wyrd;

namespace Wyrd::Editor {

	class ResourceSetLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3,
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static ResourceSetLoader::Result Load(const std::filesystem::path& path);
	};
}