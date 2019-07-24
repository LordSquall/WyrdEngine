#pragma once

#include <Osiris.h>

using namespace Osiris;

#include "core/loaders/Loaders.h"
#include "core/pipeline/Scene.h"

namespace Osiris {

	class SceneLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static SceneLoader::Result Load(std::string path, Scene& scene, FileContent content);

		static SceneLoader::Result Save(std::string path, Scene& scene, FileContent content);
	};
}