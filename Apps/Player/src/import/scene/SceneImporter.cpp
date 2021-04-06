
/* Core orisis includes */
#include <core/Log.h>

/* local project includes */
#include "import/Importer.h"

void Importer::Read(std::ifstream& stream, SceneFormat_Config& config)
{
	config.version = StringHelper(stream);
}



//#pragma once
//
//#include "loaders/GameFileImporter.h"
//
//#include <core/Log.h>
//
//static std::string StringHelper(std::ifstream& stream)
//{
//	std::string str;
//	size_t size;
//	stream.read((char*)&size, sizeof(size_t));
//	str.resize(size);
//	stream.read(&str[0], size);
//	return str;
//}
//
//void GameImporter::Read(std::ifstream& stream, GameFileFormat_Config& config)
//{
//	config.name = StringHelper(stream);
//	config.version = StringHelper(stream);
//	stream.read((char*)&config.width, sizeof(uint32_t));
//	stream.read((char*)&config.height, sizeof(uint32_t));
//}
//
//
//void GameImporter::Read(std::ifstream& stream, GameFileFormat_SceneListings& sceneListings)
//{
//	size_t sceneCount;
//	stream.read((char*)&sceneCount, sizeof(size_t));
//
//	for (size_t i = 0; i < sceneCount; i++)
//	{
//		sceneListings.sceneUIDs.push_back(StringHelper(stream));
//	}
//	sceneListings.initialScene = StringHelper(stream);
//}