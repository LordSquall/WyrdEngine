#pragma once
#include <filesystem>

/* core wyrd includes */
#include <core/pipeline/materials/Material.h>

/* local includes */


using namespace Wyrd;

namespace Wyrd::Editor {

	class MaterialLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static MaterialLoader::Result Load(const std::filesystem::path& path, std::string& name, Material* material);
																			
		static MaterialLoader::Result Save(const std::filesystem::path& path);
	};
}