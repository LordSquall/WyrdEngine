#pragma once

/* core wyrd includes */
#include <core/scene/Scene.h>

using namespace Wyrd;

namespace Wyrd::Editor {
	
	class SceneLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static SceneLoader::Result Load(const std::filesystem::path& path, Scene& scene, bool includeEditorComponents);

		static SceneLoader::Result Save(const std::filesystem::path& path, Scene& scene);
	};
}