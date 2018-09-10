#include "SpriteLayerLoader.h"

#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using namespace OrisisEngine;
using json = nlohmann::json;

SpriteLayerLoader::SpriteLayerLoader()
{
}

SpriteLayerLoader::SpriteLayerLoader(const SpriteLayerLoader &obj)
{
}

SpriteLayerLoader::~SpriteLayerLoader()
{
}

int SpriteLayerLoader::LoadFile(string filePath, ILogger* logger, SpriteSystem* spriteSystem)
{
	int layerLoadedCnt = 0;

	/* open a file stream and parse the JSON */
	ifstream fileStream(filePath);
	json j = json::parse(fileStream);
	
	/* we need to move through each of the layers defined within the file */
	const json::array_t layerArray = j["layers"];

	for (int x = 0; x < layerArray.size(); x++)
	{

		/* create a new sprite layer inside the spritesystem */
		SpriteLayer* layer = spriteSystem->CreateSpriteLayer(layerArray[x]["name"], layerArray[x]["texture"]);

		if (layer != NULL)
		{
			/* now we can begin to create each of the sprites for the layer */
			const json::array_t spriteArray = layerArray[x]["sprites"];

			for (int s = 0; s < spriteArray.size(); s++)
			{
				/* we need to find the sprite the layer to referencing */
				Sprite* sprite = spriteSystem->GetSprite(spriteArray[s]["name"]);

				if (sprite != NULL)
				{
					layer->AddEntry(sprite, spriteArray[s]["x"], spriteArray[s]["y"]);
				}
				else
				{
					logger->LogWarning("Unable to find sprite " + spriteArray[s]["name"].dump());
				}
			}

			layer->Finish();

			layerLoadedCnt++;
		}
	}

	fileStream.close();
	
	return layerLoadedCnt;
}