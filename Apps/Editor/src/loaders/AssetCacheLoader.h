#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "datamodels/resources/Resource.h"

using namespace Wyrd;

namespace Wyrd::Editor {

	class AssetCacheLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static AssetCacheLoader::Result Load(std::string path);
		static AssetCacheLoader::Result Save(std::string path);
	};
}