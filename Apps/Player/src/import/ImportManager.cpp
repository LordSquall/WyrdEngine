#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Texture.h>
#include <core/renderer/FontType.h>
#include <core/behaviour/ScriptedClass.h>

#include "import/ImportManager.h"
#include "import/ComponentSerialiserFactory.h"
#include "managers/SceneManager.h"


void ImportManager::ImportCore(const std::string& root)
{
	/* open the meta data file */
	std::ifstream core;
	core.open(root + "Core.data", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find core.dat file");
		return;
	}

	uint32_t windowWidth = 0;
	uint32_t windowHeight = 0;
	core.read((char*)&windowWidth, sizeof(uint32_t));
	core.read((char*)&windowHeight, sizeof(uint32_t));

	Application::Get().GetWindow().SetSize(windowWidth, windowHeight);

	core.close();
}

void ImportManager::ImportGame(const std::string& root)
{
	/* open the file */
	std::ifstream game;
	game.open(root + "Game.data", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find game.dat file");
		return;
	}

	/* load the scene map */
	size_t sceneCount;
	game.read((char*)&sceneCount, sizeof(size_t));

	for (int i = 0; i < sceneCount; ++i)
	{
		char sceneName[128];
		game.read((char*)&sceneName[0], sizeof(char) * 128);

		char sceneUID[64];
		game.read((char*)&sceneUID[0], sizeof(char) * 64);

		SceneManager::getInstance().AddScene(UID(sceneUID), std::string(sceneName));
	}

	char initialSceneUID[64];
	game.read((char*)&initialSceneUID[0], sizeof(char) * 64);
	SceneManager::getInstance().SetInitialScene(UID(initialSceneUID));

	/* close the file */
	game.close();
}

void ImportManager::ImportCommonBundle(const std::string& root)
{
	/* open the meta data file */
	std::ifstream common;
	common.open(root + "common.bundle", std::ios::in | std::ios::binary);
	if (common.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find common.bundle file");
		return;
	}

	size_t shaderCount;
	common.read((char*)&shaderCount, sizeof(size_t));

	for (size_t i = 0; i < shaderCount; i++)
	{
		char shaderName[32];
		common.read((char*)&shaderName[0], sizeof(char) * 32);

		size_t vertSrcSize;
		std::string vertSrc;
		size_t fragSrcSize;
		std::string fragSrc;

		common.read((char*)&vertSrcSize, sizeof(size_t));
		vertSrc.resize(vertSrcSize);
		common.read(&vertSrc[0], vertSrcSize);

		common.read((char*)&fragSrcSize, sizeof(size_t));
		fragSrc.resize(fragSrcSize);
		common.read(&fragSrc[0], fragSrcSize);

		auto newShader = std::shared_ptr<Wyrd::Shader>(Wyrd::Shader::Create());
		newShader->Build(vertSrc, fragSrc);

		Application::Get().GetResources().Shaders.insert({ shaderName, newShader });
	}

	size_t textureCount;
	common.read((char*)&textureCount, sizeof(size_t));

	for (size_t i = 0; i < textureCount; i++)
	{
		char textureUID[64];
		common.read((char*)&textureUID[0], sizeof(char) * 64);

		uint32_t width; 
		uint32_t height;

		common.read((char*)&width, sizeof(uint32_t));
		common.read((char*)&height, sizeof(uint32_t));
		
		std::vector<unsigned char> dataVector;
		dataVector.resize(width * height * 4);
		common.read((char*)&dataVector[0], sizeof(unsigned char) * (width * height * 4));

		TextureDesc textureDesc;
		textureDesc.data = &dataVector[0];
		textureDesc.width = width;
		textureDesc.height = height;
		textureDesc.channels = 4;
		textureDesc.description = "Loaded Texture";

		auto newTexture = std::shared_ptr<Wyrd::Texture>(Wyrd::Texture::Create(textureDesc));
		Application::Get().GetResources().Textures.insert({ UID(textureUID), newTexture});
	}

	size_t fontCount;
	common.read((char*)&fontCount, sizeof(size_t));

	for (size_t i = 0; i < fontCount; i++)
	{
		char fontName[64];
		common.read((char*)&fontName[0], sizeof(char) * 64);

		auto newFontType = std::make_shared<Wyrd::FontType>();

		uint32_t width;
		uint32_t height;
		uint32_t channels;

		common.read((char*)&width, sizeof(uint32_t));
		common.read((char*)&height, sizeof(uint32_t));
		common.read((char*)&channels, sizeof(uint32_t));

		std::vector<unsigned char> dataVector;
		dataVector.resize(width * height * channels);
		common.read((char*)&dataVector[0], sizeof(unsigned char) * (width * height * channels));

		TextureDesc textureDesc;
		textureDesc.data = &dataVector[0];
		textureDesc.width = width;
		textureDesc.height = height;
		textureDesc.channels = 1;
		textureDesc.uvWrapping = TextureUVWrap::CLAMP_TO_EDGE;
		textureDesc.description = "Loaded Texture";

		newFontType->Texture = std::shared_ptr<Wyrd::Texture>(Wyrd::Texture::Create(textureDesc));

		size_t characterCount;
		common.read((char*)&characterCount, sizeof(size_t));
		for (size_t c = 0; c < characterCount; c++)
		{
			Wyrd::FontType::Character character;
			character.Advance;

			common.read((char*)&character.Advance, sizeof(unsigned int));
			common.read((char*)&character.Bearing, sizeof(glm::ivec2));
			common.read((char*)&character.Size, sizeof(glm::ivec2));
			common.read((char*)&character.uv1, sizeof(glm::vec2));
			common.read((char*)&character.uv2, sizeof(glm::vec2));

			newFontType->GetCharacters().insert(std::pair<char, FontType::Character>(c, character));
		}

		Application::Get().GetResources().FontTypes.insert({ fontName, newFontType });
		
	}

	{
		size_t fileCount;
		common.read((char*)&fileCount, sizeof(size_t));

		std::vector<UID> scriptFilesUID;
		std::vector<std::string> scriptFilesNames;
		for (size_t i = 0; i < fileCount; i++)
		{
			char fileNamebuffer[64];
			common.read((char*)&fileNamebuffer, sizeof(char) * 64);

			char uuidBuffer[64];
			common.read((char*)&uuidBuffer, sizeof(char) * 64);

			scriptFilesUID.push_back(UID(uuidBuffer));
			scriptFilesNames.push_back(fileNamebuffer);
		}

		size_t coreLibSize;
		common.read((char*)&coreLibSize, sizeof(size_t));

		std::vector<char> coreLibData;
		coreLibData.resize(coreLibSize);
		common.read((char*)&coreLibData[0], sizeof(char) * coreLibSize);

		size_t clientLibSize;
		common.read((char*)&clientLibSize, sizeof(size_t));

		std::vector<char> clientLibData;
		clientLibData.resize(clientLibSize);
		common.read((char*)&clientLibData[0], sizeof(char) * clientLibSize);

		Application::Get().GetBehaviour().LoadBehaviourModel(scriptFilesNames, coreLibData, clientLibData);

		for (int i = 0; i < scriptFilesNames.size(); ++i)
		{
			Application::Get().GetBehaviour().GetCustomClass(scriptFilesNames[i])->SetUID(scriptFilesUID[i]);
		}
	}
	common.close();
}

std::unique_ptr<Wyrd::Scene> ImportManager::ImportScene(const std::string& root, const Wyrd::UID& sceneUID)
{
	/* open the meta data file */
	std::ifstream scene;
	scene.open(root + "bundles/" + sceneUID.str().c_str() + ".bundle", std::ios::in | std::ios::binary);
	if (scene.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find bundles/{0}.bundle file", sceneUID);
		return nullptr;
	}

	std::unique_ptr<Wyrd::Scene> newScene = std::make_unique<Wyrd::Scene>();
	newScene->Initialise();

	Entity cameraEntity;
	scene.read((char*)&cameraEntity, sizeof(Entity));
	newScene->SetPrimaryCameraEntity(cameraEntity);

	size_t entityCount;
	scene.read((char*)&entityCount, sizeof(size_t));

	for (size_t i = 0; i < entityCount; i++)
	{
		Entity entity;
		uint64_t componentMask;

		scene.read((char*)&entity, sizeof(Entity));
		scene.read((char*)&componentMask, sizeof(uint64_t));
		
		if (entity > newScene->entities.size())
			newScene->entities.resize(entity);

		newScene->entities[entity - 1] = { entity, (ComponentMask)componentMask };
	}

	size_t componentCount;
	scene.read((char*)&componentCount, sizeof(size_t));

	for (size_t poolIndex = 0; poolIndex < componentCount; poolIndex++)
	{
		char name[64];
		size_t elementSize;
		size_t elementCount;

		scene.read((char*)&name, sizeof(char) * 64);
		scene.read((char*)&elementSize, sizeof(size_t));
		scene.read((char*)&elementCount, sizeof(size_t));

		for (size_t elementIndex = 0; elementIndex < elementCount; elementIndex++)
		{
			ComponentSerialiserFactory::Deserialise(name, scene, &(newScene->componentPools[poolIndex]->data[elementIndex * elementSize]));
			newScene->componentPools[poolIndex]->count++;
		}
	}


	for (auto& e : newScene->entities)
	{
		WYRD_CORE_INFO("Entity ID: {0} Mask: {1}", e.id, e.mask);
	}

	for (auto& cp : newScene->componentPools)
	{
		WYRD_CORE_INFO("Component Pool Name: {0} ElementSize: {1}", cp->name, cp->elementSize);
	}

	return std::move(newScene);
}