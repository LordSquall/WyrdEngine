#pragma once

/* core wyrd includes */

/* local includes */
#include "datamodels/Project.h"

using namespace Wyrd;

namespace Wyrd::Editor {

	class ProjectLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static ProjectLoader::Result Load(std::string path, Project& project);
																			
		static ProjectLoader::Result Save(std::string path, Project& project);
	};
}