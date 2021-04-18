#pragma once

#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Shader.h>
#include <core/scene/components/IBaseComponent.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/CameraComponent.h>

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


namespace Osiris::Editor
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

		OSR_CORE_TRACE("Exported Game Files");
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

		commonBundle.close();
	}

	void ExportManager::GenerateSceneBundleFile(const Osiris::UID& sceneUID, const std::string& dir)
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
		for (auto& l : scene.GetLayers())
		{
			SceneFormat_Layer layer;
			layer.name = l->GetName();

			for (auto& go : l->GetGameObjects())
			{
				SceneFormat_GameObject gameObject;
				gameObject.name = go->name;
				gameObject.uid = go->uid;

				Transform2DComponent* transform2DComponent = dynamic_cast<Transform2DComponent*>(go->transform.get());
				gameObject.transformType = 1;
				gameObject.transformDef.transform2D.x = transform2DComponent->position.x;  
				gameObject.transformDef.transform2D.y = transform2DComponent->position.y;
				gameObject.transformDef.transform2D.angle = transform2DComponent->rotation;
				gameObject.transformDef.transform2D.scaleX = transform2DComponent->scale.y;
				gameObject.transformDef.transform2D.scaleY = transform2DComponent->scale.y;

				for (auto& c : go->components)
				{
					SceneFormat_Component component;
					component.type = (int32_t)c->GetType();

					switch ((SceneComponentType)component.type)
					{
					case SceneComponentType::SpriteRenderer:
						{
							SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(c.get());
							component.componentDef.sprite.x = spriteComponent->position.x;
							component.componentDef.sprite.y = spriteComponent->position.y;
							component.componentDef.sprite.width = spriteComponent->size.x;
							component.componentDef.sprite.height = spriteComponent->size.y;
						}
						break;
					case SceneComponentType::CameraComponent:
						{
							CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(c.get());
							component.componentDef.camera.uid = cameraComponent->GetCameraUID();
							component.componentDef.camera.size = cameraComponent->GetSize();
							component.componentDef.camera.width = cameraComponent->GetCamera().GetViewport().size.x;
							component.componentDef.camera.height = cameraComponent->GetCamera().GetViewport().size.y;
						}
						break;
					}
					gameObject.components.push_back(component);
				}
				gameObject.components_cnt = go->components.size();

				layer.gameObjects.push_back(gameObject);
			}
			layer.gameObjects_cnt = l->GetGameObjects().size();

			layerConfig.layers.push_back(layer);
		}
		layerConfig.layers_cnt = scene.GetLayers().size();
		Write(sceneBundle, layerConfig);

		sceneBundle.close();
	}
}