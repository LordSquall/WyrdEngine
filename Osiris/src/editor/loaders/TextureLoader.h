#pragma once

#include <Osiris.h>
#include "core/loaders/Loaders.h"
#include "editor/datamodels/resources/TextureRes.h"


namespace Osiris::Editor {

	class TextureLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3,
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static TextureLoader::Result Load(std::string path, TextureRes& texture);
	};
}