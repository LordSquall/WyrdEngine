#pragma once
#include <filesystem>

/* core wyrd includes */
#include <core/UID.h>

/* local includes */
#include "datamodels/resources/Resource.h"

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

		static ResourceSetLoader::Result Load(const std::filesystem::path& path, std::map<UID, std::shared_ptr<Resource>>& resourceMap);
	};
}