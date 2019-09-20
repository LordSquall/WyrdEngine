#pragma once

#include <Osiris.h>
#include "core/loaders/Loaders.h"
#include "editor/datamodels/Project.h"

using namespace Osiris;

namespace Osiris::Editor {

	class ProjectLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static ProjectLoader::Result Load(std::string path, Project& project, FileContent content);

		static ProjectLoader::Result Save(std::string path, Project& project, FileContent content);
	};
}