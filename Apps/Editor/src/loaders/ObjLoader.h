#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/pipeline/Vertex3D.h>

using namespace Wyrd;

namespace Wyrd::Editor {

	class ObjLoader
	{
	public:

		enum Result
		{
			Success = 0, FileNotFound = 1, FileInUse = 2, FileMalformed = 3, 
			DirectoryNotFound = 4, FileAlreadyExists = 5, InsufficientSpace = 6
		};

		static ObjLoader::Result Load(std::string path, std::vector<Vertex3D>* vertices, std::vector<uint32_t>* indices = nullptr, std::map<std::string, std::pair<uint32_t, uint32_t>>* vertexGroups = nullptr, float scaleFactor = 1.0f);
	};
}