#pragma once

/* core osiris includes */
#include <Osiris.h>

/* local includes */
#include "datamodels/Project.h"
#include "support/IconLibrary.h"

using namespace Osiris;

namespace Osiris::Editor {

	class IconSetLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static IconSetLoader::Result Load(std::string path, std::shared_ptr<IconSet> iconSet);
	};
}