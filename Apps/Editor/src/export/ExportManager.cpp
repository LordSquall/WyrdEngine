#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/behaviour/Behaviour.h>
#include <core/renderer/Shader.h>

/* export headers */
#include "CoreFormat.h"
#include "CoreFunctions.h"
#include "GameFormat.h"
#include "GameFunctions.h"
#include "BundleFormat.h"
#include "BundleFunctions.h"
#include "SceneFormat.h"
#include "SceneFunctions.h"
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

		/* bundle configuration header */
		CoreFormat_Config config;
		config.version = "1.0.0";
		Write(core, config);

		core.close();
	}

	void ExportManager::GenerateGameFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		/* Game Configuration */
		GameFormat_Config config;
		config.version = "1.0.0";
		config.name = project->name;
		Write(game, config);

		/* Display Configuration */
		GameFormat_Display display;
		display.width = (uint32_t)project->GetExportSettings().width;
		display.height = (uint32_t)project->GetExportSettings().height;
		Write(game, display);

		/* Scene Listings */
		GameFormat_SceneListings sceneListings;
		for (auto& exportableScene : project->GetExportSettings().exportableScenes)
		{
			sceneListings.sceneUIDs.push_back(exportableScene.first);
		}
		sceneListings.sceneUIDs_cnt = project->GetExportSettings().exportableScenes.size();
		sceneListings.initialScene = project->GetExportSettings().initialScene;
		Write(game, sceneListings);

		game.close();
	}

	void ExportManager::GenerateCommonBundleFile()
	{
		auto resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		std::ofstream commonBundle;
		commonBundle.open(Utils::GetBuildsFolder() + "/common.bundle", std::ios::out | std::ios::binary);

		/* bundle configuration header */
		BundleFormat_Config config;
		config.version = "1.0.0";
		Write(commonBundle, config);

		/* shader configuration */
		BundleFormat_ShaderConfig shaderConfig;
		for (auto& s : Application::Get().GetResources().Shaders)
		{
			BundleFormat_Shader shader;
			shader.uid = UIDUtils::Create(); // TODO
			shader.vertexSrc = s.second->GetSource(ShaderStage::Vertex);
			shader.fragmentSrc = s.second->GetSource(ShaderStage::Fragment);
			shaderConfig.shaders.push_back(shader);
		}
		shaderConfig.shaders_cnt = Application::Get().GetResources().Shaders.size();
		Write(commonBundle, shaderConfig);

		/* texture configuration */
		BundleFormat_TextureConfig textureConfig;

		uint32_t textureCount = 0;

		/* add the default texture */
		auto defaultTexture = Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)];
		BundleFormat_Texture defaultTextureFormat;
		defaultTextureFormat.uid = defaultTexture->GetUID();
		defaultTextureFormat.width = defaultTexture->GetWidth();
		defaultTextureFormat.height = defaultTexture->GetHeight();
		defaultTextureFormat.channels = 4;
		defaultTextureFormat.data_cnt = defaultTextureFormat.width * defaultTextureFormat.height * defaultTextureFormat.channels;

		for (int i = 0; i < defaultTextureFormat.data_cnt; i++)
		{
			defaultTextureFormat.data.push_back(defaultTexture->GetData()[i]);
		}
		textureConfig.textures.push_back(defaultTextureFormat);
		textureCount++;

		for (auto& r : resourceService->GetResources())
		{
			TextureRes* textureRes = dynamic_cast<TextureRes*>(r.second.get());
			if (textureRes != nullptr)
			{
				BundleFormat_Texture texture;
				texture.uid = textureRes->GetTexture()->GetUID();
				texture.width = textureRes->GetWidth();
				texture.height = textureRes->GetHeight();
				texture.channels = textureRes->GetChannels();
				texture.data_cnt = texture.width * texture.height * texture.channels;

				for (int i = 0; i < texture.data_cnt; i++)
				{
					texture.data.push_back(textureRes->GetData()[i]);
				}

				textureConfig.textures.push_back(texture);
				textureCount++;
			}
		}
		textureConfig.textures_cnt = textureCount;
		Write(commonBundle, textureConfig);

		/* managed libraries configuration */
		BundleFormat_ManagedLibConfig managedLibConfig;

		//TODO
		std::string coreAPIPath = "C:/Projects/Wyrd/WyrdEngine/lib/Debug/WyrdAPI/WyrdAPI.dll";
		std::string clientAPIPath = Utils::GetPath(workspaceService->GetLoadedProjectPath()) + "\\" + workspaceService->GetCurrentProject()->name + ".dll";

		BundleFormat_ManagedLib managedCoreLib;
		managedCoreLib.name = "Core";

		std::ifstream coreManagedLibFileStream(coreAPIPath.c_str(), std::ios::binary);
		managedCoreLib.data.assign((std::istreambuf_iterator<char>(coreManagedLibFileStream)), (std::istreambuf_iterator<char>()));
		managedCoreLib.data_cnt = managedCoreLib.data.size();
		coreManagedLibFileStream.close();

		BundleFormat_ManagedLib managedClientLib;
		managedClientLib.name = "Client";

		std::ifstream clientManagedLibFileStream(clientAPIPath.c_str(), std::ios::binary);
		managedClientLib.data.assign((std::istreambuf_iterator<char>(clientManagedLibFileStream)), (std::istreambuf_iterator<char>()));
		managedClientLib.data_cnt = managedClientLib.data.size();
		clientManagedLibFileStream.close();

		managedLibConfig.managedLibraries.push_back(managedCoreLib);
		managedLibConfig.managedLibraries.push_back(managedClientLib);
		managedLibConfig.managedLibraries_cnt = managedLibConfig.managedLibraries.size();

		std::vector<std::string> scriptFiles;
		for (auto& [key, value] : resourceService->GetResources())
		{
			auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
			if (downcastedPtr)
			{
				managedLibConfig.classes.push_back(value->GetName());
				managedLibConfig.classesUIDs.push_back(value->GetResourceID());
			}
		}
		managedLibConfig.classes_cnt = managedLibConfig.classes.size();
		managedLibConfig.classesUIDs_cnt = managedLibConfig.classesUIDs.size();

		Write(commonBundle, managedLibConfig);

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
		SceneLoader::Load(sceneRes->GetPath(), scene);

		/* scene configuration header */
		SceneFormat_Config config;
		config.version = "1.0.0";
		config.name = sceneRes->GetName();
		config.camera = scene.GetPrimaryCameraUID();
		Write(sceneBundle, config);

		/* layer configuration */
		SceneFormat_LayerConfig layerConfig;
		Write(sceneBundle, layerConfig);

		sceneBundle.close();
	}
}