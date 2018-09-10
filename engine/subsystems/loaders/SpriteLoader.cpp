#include "SpriteLoader.h"

#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using namespace OrisisEngine;
using json = nlohmann::json;

SpriteLoader::SpriteLoader()
{
}

SpriteLoader::SpriteLoader(const SpriteLoader &obj)
{
}

SpriteLoader::~SpriteLoader()
{
}

int SpriteLoader::LoadFile(string filePath, ILogger* logger, SpriteSystem* spriteSystem)
{
	int spritesLoadedCnt = 0;

	/* open a file stream and parse the JSON */
	ifstream fileStream(filePath);
	json j = json::parse(fileStream);
	
	/* we need to move through each of the textures defined within the file */
	const json::array_t textureArray = j["textures"];

	for (int x = 0; x < textureArray.size(); x++) 
	{
		/* load the texture into the sprite system */
		Texture* newTexture = spriteSystem->LoadTextureFromFile(textureArray[x]["filename"], textureArray[x]["name"]);
		newTexture->Width(textureArray[x]["width"]);
		newTexture->Height(textureArray[x]["height"]);

		/* now we can begin to create each of the sprites for the texture */
		const json::array_t spriteArray = textureArray[x]["sprites"];

		for (int s = 0; s < spriteArray.size(); s++)
		{
			int w = spriteArray[s]["width"];
			int h = spriteArray[s]["height"];
			int x = spriteArray[s]["x"];
			int y = spriteArray[s]["y"];

			Sprite* newSprite = spriteSystem->CreateSprite(spriteArray[s]["name"], spriteArray[s]["width"], spriteArray[s]["height"], newTexture->Name());
			
			if (newSprite != NULL)
			{
				newSprite->TextureU[0] = (float)x / (float)newTexture->Width();
				newSprite->TextureV[0] = (float)y / (float)newTexture->Height();

				newSprite->TextureU[1] = (float)x / (float)newTexture->Width();
				newSprite->TextureV[1] = (float)(y + h) / (float)newTexture->Height();

				newSprite->TextureU[2] = (float)(x + w) / (float)newTexture->Width();
				newSprite->TextureV[2] = (float)(y + h) / (float)newTexture->Height();

				newSprite->TextureU[3] = (float)(x + w) / (float)newTexture->Width();
				newSprite->TextureV[3] = (float)y / (float)newTexture->Height();

				spritesLoadedCnt++;
			}
		}
	}

	fileStream.close();
	
	return spritesLoadedCnt;
}