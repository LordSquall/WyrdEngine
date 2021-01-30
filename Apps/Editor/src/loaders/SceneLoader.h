#pragma once

/* core osiris includes */
#include <core/scene/Scene.h>

using namespace Osiris;

namespace Osiris::Editor {
	
	class SceneLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static SceneLoader::Result Load(std::string path, Scene& scene);

		static SceneLoader::Result Save(std::string path, Scene& scene);
	};
}