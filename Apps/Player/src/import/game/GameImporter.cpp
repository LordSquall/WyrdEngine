
/* Core orisis includes */
#include <core/Log.h>

/* local project includes */
#include "import/Importer.h"

void Importer::Read(std::ifstream& stream, GameFormat_Config& config)
{
	config.version = StringHelper(stream);
	config.name = StringHelper(stream);
}

void Importer::Read(std::ifstream& stream, GameFormat_Display& display)
{
	display.width = UIntHelper(stream);
	display.height = UIntHelper(stream);
}

void Importer::Read(std::ifstream& stream, GameFormat_SceneListings& sceneListings)
{
	uint32_t sceneCnt = UIntHelper(stream);

	for (int i = 0; i < sceneCnt; i++)
	{
		sceneListings.sceneUIDs.push_back(StringHelper(stream));
	}

	sceneListings.initialScene = StringHelper(stream);
}