#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "datamodels/resources/Resource.h"

using namespace Wyrd;

namespace Wyrd::Editor {

	class SpriteSheetLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static SpriteSheetLoader::Result Load(const std::filesystem::path& path, std::vector<ImageArea>& imageAreas);
		static SpriteSheetLoader::Result Save(const std::filesystem::path& path, const std::vector<ImageArea>& imageAreas);
	};
}