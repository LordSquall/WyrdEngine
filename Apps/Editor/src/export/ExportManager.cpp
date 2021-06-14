#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/behaviour/Behaviour.h>
#include <core/renderer/Shader.h>

/* export headers */
#include "loaders/SceneLoader.h"

#include "ExportManager.h"
#include "services/ServiceManager.h"
#include "services/WorkspaceService.h"
#include "support/Utils.h"


namespace Wyrd::Editor
{
	void ExportManager::Export()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();
		std::shared_ptr<Scene> scene = workspaceService->GetLoadedScene();

		std::ofstream commonBundle;
		
		std::string bundlesDir = Utils::GetBuildsFolder() + "/bundles/";
		Utils::CreateFolder(bundlesDir);

		GenerateCoreFile();
		GenerateGameFile();
		GenerateCommonBundleFile();

		for (auto& exportableScene : project->GetExportSettings().exportableScenes)
		{
			GenerateSceneBundleFile(exportableScene.first, bundlesDir);
		}

		WYRD_CORE_TRACE("Exported Game Files");
	}

	void ExportManager::GenerateCoreFile()
	{
		std::ofstream core;
		core.open(Utils::GetBuildsFolder() + "/Core.dat", std::ios::out | std::ios::binary);

		core.close();
	}

	void ExportManager::GenerateGameFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		game.close();
	}

	void ExportManager::GenerateCommonBundleFile()
	{
		auto resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		std::ofstream commonBundle;
		commonBundle.open(Utils::GetBuildsFolder() + "/common.bundle", std::ios::out | std::ios::binary);

		std::map<UID, TextureRes*> textures;

		for (auto& res : resourceService->GetResources())
		{
			switch (res.second->GetType())
			{
			case ResourceType::TEXTURE:
				textures.insert({ res.first, std::dynamic_pointer_cast<TextureRes>(res.second).get() });
				break;
			}
		}

		/* Disabled shader export until resources are resolved */
		size_t shaderCount = Application::Get().GetResources().Shaders.size();
		shaderCount = 0;
		commonBundle.write((char*)&shaderCount, sizeof(size_t));
		/*for (auto& s : Application::Get().GetResources().Shaders)
		{
			char shaderName[32];
			strcpy(shaderName, s.first.c_str());

			std::string vertSrc = s.second->GetSource(ShaderStage::Vertex); 
			std::string fragSrc = s.second->GetSource(ShaderStage::Fragment);

			size_t vertSrcSize = vertSrc.size();
			size_t fragSrcSize = fragSrc.size();

			commonBundle.write((char*)shaderName, sizeof(char) * 32);
			commonBundle.write((char*)&vertSrcSize, sizeof(size_t));
			commonBundle.write((char*)vertSrc.c_str(), sizeof(char) * vertSrcSize);
			commonBundle.write((char*)&fragSrcSize, sizeof(size_t));
			commonBundle.write((char*)fragSrc.c_str(), sizeof(char) * fragSrcSize);
		}*/

		size_t textureCount = textures.size();
		commonBundle.write((char*)&textureCount, sizeof(size_t));
		for (auto& t : textures)
		{
			char textureUID[64];
			strcpy(textureUID, t.first.str().c_str());

			uint32_t width = (uint32_t)t.second->GetWidth();
			uint32_t height = (uint32_t)t.second->GetHeight();
			unsigned char* data = t.second->GetData();

			commonBundle.write(&textureUID[0], sizeof(char) * 64);
			commonBundle.write((char*)&width, sizeof(uint32_t));
			commonBundle.write((char*)&height, sizeof(uint32_t));
			commonBundle.write((char*)data, sizeof(unsigned char) * (width * height * 4));
		}

		{
			std::map<UID, std::string> scriptFiles;
			for (auto& [key, value] : resourceService->GetResources())
			{
				auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
				if (downcastedPtr)
				{
					scriptFiles.insert({ key , value->GetPath() });
				}
			}

			size_t fileCount = scriptFiles.size();
			commonBundle.write((char*)&fileCount, sizeof(size_t));
			for (auto& file : scriptFiles)
			{
				char fileNamebuffer[64];
				strcpy(fileNamebuffer, Utils::GetFilename(file.second).c_str());
				commonBundle.write((char*)&fileNamebuffer, sizeof(char) * 64);

				char uuidBuffer[64];
				strcpy(uuidBuffer, file.first.str().c_str());
				commonBundle.write((char*)&uuidBuffer, sizeof(char) * 64);
			}

			std::string coreLibraryLocation = "C:/Projects/Wyrd/WyrdEngine/lib/Debug/WyrdAPI/WyrdAPI.dll";
			std::string clientLibraryLocation = Utils::GetPath(workspaceService->GetLoadedProjectPath()) + "\\" + workspaceService->GetCurrentProject()->name + ".dll";

			std::ifstream coreLibFileStream(coreLibraryLocation.c_str(), std::ios::binary);
			std::vector<char> coreLibData((std::istreambuf_iterator<char>(coreLibFileStream)), (std::istreambuf_iterator<char>()));
			coreLibFileStream.close();

			std::ifstream clientLibFileStream(clientLibraryLocation.c_str(), std::ios::binary);
			std::vector<char> clientLibData((std::istreambuf_iterator<char>(clientLibFileStream)), (std::istreambuf_iterator<char>()));
			clientLibFileStream.close();

			size_t coreLibFileSize = coreLibData.size(); 
			size_t clientLibFileSize = clientLibData.size();

			commonBundle.write((char*)&coreLibFileSize, sizeof(size_t));
			commonBundle.write(&coreLibData[0], sizeof(char) * coreLibFileSize);

			commonBundle.write((char*)&clientLibFileSize, sizeof(size_t));
			commonBundle.write(&clientLibData[0], sizeof(char) * clientLibFileSize);
		}

		commonBundle.close();
	}

	void ExportManager::GenerateSceneBundleFile(const Wyrd::UID& sceneUID, const std::string& dir)
	{
		std::ofstream sceneBundle;
		sceneBundle.open(dir + sceneUID.str() + ".bundle", std::ios::out | std::ios::binary);

		/* Load the matching scene */
		auto resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		auto sceneRes = resourceService->GetResourceByID<SceneRes>(sceneUID);

		Scene scene;
		scene.Initialise();
		SceneLoader::Load(sceneRes->GetPath(), scene);

		Entity cameraEntity = scene.GetPrimaryCameraEntity();
		sceneBundle.write((char*)&cameraEntity, sizeof(Entity));

		size_t entityCount = scene.entities.size();

		sceneBundle.write((char*)&entityCount, sizeof(size_t));
		for (auto& e : scene.entities)
		{
			uint64_t mask = e.mask.to_ullong();

			sceneBundle.write((char*)&e, sizeof(Entity));
			sceneBundle.write((char*)&mask, sizeof(uint64_t));
		}

		size_t componentPoolCount = scene.componentPools.size();

		sceneBundle.write((char*)&componentPoolCount, sizeof(size_t));
		for (auto& cp : scene.componentPools)
		{
			char poolName[64];
			size_t poolElementSize;
			size_t poolElementCount;

			strcpy(&poolName[0], cp->name.c_str());
			poolElementSize = cp->elementSize;
			poolElementCount = cp->count;

			sceneBundle.write((char*)&poolName, sizeof(char) * 64);
			sceneBundle.write((char*)&poolElementSize, sizeof(size_t));
			sceneBundle.write((char*)&poolElementCount, sizeof(size_t));

			sceneBundle.write(cp->data, poolElementSize * poolElementCount);
		}

		sceneBundle.close();
	}
}