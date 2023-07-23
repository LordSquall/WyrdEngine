#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Texture.h>
#include <core/renderer/Material.h>
#include <core/renderer/Mesh.h>
#include <core/renderer/FontType.h>
#include <core/behaviour/ScriptedClass.h>
#include <serial/ComponentSerialiserFactory.h>
#include <serial/ResourceSerialisers.h>

#include "import/ImportManager.h"
#include "managers/SceneManager.h"


bool ImportManager::ImportCore(const std::string& root)
{
	/* open the meta data file */
	std::ifstream core;
	core.open(root + "Core.data", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find core.dat file");
		return false;
	}

	uint32_t windowWidth = 0;
	uint32_t windowHeight = 0;
	core.read((char*)&windowWidth, sizeof(uint32_t));
	core.read((char*)&windowHeight, sizeof(uint32_t));

	Application::Get().GetWindow().SetSize(windowWidth, windowHeight);

	core.close();

	return true;
}

bool ImportManager::ImportGame(const std::string& root)
{
	/* open the file */
	std::ifstream game;
	game.open(root + "Game.data", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find game.dat file");
		return false;
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

		//SceneManager::getInstance().AddScene(UID(sceneUID), std::string(sceneName));
	}

	char initialSceneUID[64];
	game.read((char*)&initialSceneUID[0], sizeof(char) * 64);
	SceneManager::getInstance().SetInitialScene(UID(initialSceneUID));

	/* close the file */
	game.close();

	return true;
}

bool ImportManager::ImportCommonBundle(const std::string& root)
{
	/* open the meta data file */
	std::ifstream common;
	common.open(root + "common.bundle", std::ios::in | std::ios::binary);
	if (common.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find common.bundle file");
		return false;
	}

	WYRD_CORE_TRACE("Import Core Resources:");

	size_t textureCount;
	common.read((char*)&textureCount, sizeof(size_t));
	WYRD_CORE_TRACE("\tTextures: {0}", textureCount);

	for (size_t i = 0; i < textureCount; i++)
	{
		auto newTexture = DeserialiseTexture(common);

		Application::Get().GetResources().Textures.insert({ newTexture->GetUID(), newTexture });
		WYRD_CORE_TRACE("\t\t{0} -> {1}", newTexture->GetUID().str(), newTexture->GetName());
	}

	size_t modelCount;
	common.read((char*)&modelCount, sizeof(size_t));
	WYRD_CORE_TRACE("\tModel: {0}", modelCount);

	for (size_t i = 0; i < modelCount; i++)
	{
		std::shared_ptr<Mesh> newMesh = DeserialiseModel(common);

		Application::Get().GetResources().Meshs.insert({ newMesh->GetUID(), newMesh });
		WYRD_CORE_TRACE("\t\t{0} -> {1}", newMesh->GetUID().str(), newMesh->GetName());
	}

	size_t shaderCount;
	common.read((char*)&shaderCount, sizeof(size_t));
	WYRD_CORE_TRACE("\tShader: {0}", shaderCount);

	for (size_t i = 0; i < shaderCount; i++)
	{
		std::shared_ptr<Shader> newShader = DeserialiseShader(common);

		Application::Get().GetResources().Shaders.insert({ newShader->GetName(), newShader });
		WYRD_CORE_TRACE("\t\t{0}", newShader->GetName());
	}
	
	size_t materialCount;
	common.read((char*)&materialCount, sizeof(size_t));
	WYRD_CORE_TRACE("\tMaterials: {0}", materialCount);

	for (size_t i = 0; i < materialCount; i++)
	{
		std::shared_ptr<Material> newMaterial = DeserialiseMaterial(common);

		Application::Get().GetResources().Materials.insert({ newMaterial->GetUID(), newMaterial });
		WYRD_CORE_TRACE("\t\t{0} -> {1}", newMaterial->GetUID().str(), newMaterial->GetName());
	}


	///////////////////////
	/* Build Shaders */
	for (auto& shader : Application::Get().GetResources().Shaders)
	{
		shader.second->Build(shader.second->GetSource(ShaderStage::Vertex), shader.second->GetSource(ShaderStage::Fragment));
	}
	
	
	
	{
		size_t fileCount;
		common.read((char*)&fileCount, sizeof(size_t));
	
		std::vector<UID> scriptFilesUID;
		std::vector<std::filesystem::path> scriptFilesNames;
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
			Application::Get().GetBehaviour().GetCustomClass(scriptFilesNames[i].string())->SetUID(scriptFilesUID[i]);
		}
	}
	common.close();

	return true;
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