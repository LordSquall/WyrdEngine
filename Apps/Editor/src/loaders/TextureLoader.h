#pragma once

/* core wyrd includes */

/* local includes */
#include "datamodels/resources/TextureRes.h"

namespace Wyrd::Editor {

	class TextureLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3,
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static TextureLoader::Result Load(const std::filesystem::path& filepath, int32_t& width, int32_t& height, uint32_t& channels, unsigned char** data);
	};
}