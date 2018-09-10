#include "SampleScene.h"

#include "subsystems\sprites\SpriteSystem.h"

SampleScene::SampleScene(Framework* framework) : Scene(framework)
{

}

SampleScene::SampleScene(const SampleScene &SampleScene)
{

}

SampleScene::~SampleScene()
{

}

void SampleScene::OnSceneLoaded()
{
	/* retrieve a pointer to the frameworks sprite system */
	SpriteSystem* spriteSystem = _framework->GetSpriteSystem();

	spriteSystem->LoadSpritesFromFile("../application/spritedata1.json");
	spriteSystem->LoadLayersFromFile("../application/layer1.json");

	spriteSystem->LoadSpritesFromFile("../application/spritedata2.json");
	spriteSystem->LoadLayersFromFile("../application/layer2.json");

	std::map<string, SpriteLayer*> layers = spriteSystem->GetAllLayers();
	for(std::map<string, SpriteLayer*>::iterator it = layers.begin(); it != layers.end(); ++it)
	{
		_logger->LogDebug(it->second->Name);

		it->second->OutputLayer(_logger);
	}
}

void SampleScene::OnSceneClosed()
{
	_logger->LogInfo("SampleScene: OnSceneClosed Called!");
}
