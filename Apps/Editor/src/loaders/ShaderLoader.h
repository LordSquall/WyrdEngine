#pragma once
#include <filesystem>

/* core wyrd includes */
#include <jsonxx.h>

/* local includes */
#include "datamodels/resources/ShaderRes.h"


using namespace Wyrd;

namespace Wyrd::Editor {

	class ShaderLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static ShaderLoader::Result Load(const std::filesystem::path& path, std::string& name, std::string& vs, std::string& fs);
																			
		static ShaderLoader::Result Load(const jsonxx::Object& obj, std::string& name, std::string& vs, std::string& fs);

		static ShaderLoader::Result Save(const std::filesystem::path& path, ShaderRes* shaderRes);
	};
}