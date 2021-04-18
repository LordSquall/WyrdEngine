#pragma once

#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/renderer/Shader.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/SceneComponentFactory.h>

#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include "CoreFormat.h"
#include "CoreFunctions.h"
#include "GameFormat.h"
#include "GameFunctions.h"
#include "BundleFormat.h"
#include "BundleFunctions.h"
#include "SceneFormat.h"
#include "SceneFunctions.h"

void ImportManager::ImportCore(const std::string& root)
{
	/* open the meta data file */
	std::ifstream core;
	core.open(root + "Core.dat", std::ios::in | std::ios::binary);
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find core.dat file");
		return;
	}

	/* core file configuration */
	CoreFormat_Config config;
	Read(core, config);

	OSR_TRACE("Core Config Version: {0}", config.version);

	core.close();
}

void ImportManager::ImportGame(const std::string& root)
{
	/* open the file */
	std::ifstream game;
	game.open(root + "Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		OSR_CORE_ERROR("Unable to find game.dat file");
		return;
	}

	/* game file configuration */
	GameFormat_Config config;
	Read(game, config);

	OSR_TRACE("Game Config Version: {0}", config.version);
	
	Osiris::Application::Get().GetWindow().SetTitle(config.name + " - " + config.version);

	///* display configuration */
	GameFormat_Display display;
	Read(game, display);

	Osiris::Application::Get().GetWindow().SetSize(display.width, display.height);

	Application::Get().GetRenderer().SetViewport(
		Application::Get().GetWindow().GetWidth(),
		Application::Get().GetWindow().GetHeight()
	);

	/* scene listings */
	GameFormat_SceneListings sceneListings;
	Read(game, sceneListings);

	for (auto& listing : sceneListings.sceneUIDs) OSR_TRACE("Scene Listing: {0}", listing);

	OSR_TRACE("Initial Scene: {0}", sceneListings.initialScene);
	SceneManager::getInstance().SetInitialScene(sceneListings.initialScene);

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
		OSR_CORE_ERROR("Unable to find common.bundle file");
		return;
	}

	/* common file configuration */
	BundleFormat_Config config;
	Read(common, config);

	OSR_TRACE("Common Config Version: {0}", config.version);

	/* shader configuration */
	BundleFormat_ShaderConfig shaderConfig;
	Read(common, shaderConfig);

	OSR_TRACE("Shader Config Count: {0}", shaderConfig.shaders_cnt);

	for (auto& s : shaderConfig.shaders)
	{
		std::shared_ptr<Osiris::Shader> shader = std::shared_ptr<Osiris::Shader>(Osiris::Shader::Create());
		if (shader->Build(s.vertexSrc, s.fragmentSrc))
		{
			Osiris::Application::Get().GetResources().Shaders[s.uid.str()] = shader;
			OSR_INFO("Shader Compiled and Added! [{0}]", s.uid.str());
		}
		else
		{
			OSR_ERROR("Unable to Compile Shader [{0}]", s.uid.str());
		}
	}

	common.close();
}

std::unique_ptr<Osiris::Scene> ImportManager::ImportScene(const std::string& root, const Osiris::UID& sceneUID)
{
	/* open the meta data file */
	std::ifstream scene;
	scene.open(root + "bundles/" + sceneUID.str().c_str() + ".bundle", std::ios::in | std::ios::binary);
	if (scene.good() == false)
	{
		OSR_CORE_ERROR("Unable to find bundles/{0}.bundle file", sceneUID);
		return nullptr;
	}

	/* common file configuration */
	SceneFormat_Config config;
	Read(scene, config);

	OSR_TRACE("Scene Config Version: {0} Name: {1}", config.version, config.name);

	/* create new scene object to begin build */
	std::unique_ptr<Osiris::Scene> newScene = std::make_unique<Osiris::Scene>();

	/* primary camera */
	newScene->SetPrimaryCameraUID(config.camera);

	/* Layer configuration */
	SceneFormat_LayerConfig layerConfig;
	Read(scene, layerConfig);

	for (auto& l : layerConfig.layers)
	{
		auto& layer = newScene->AddLayer(std::make_unique<SceneLayer2D>(l.name));
		OSR_TRACE(" -> Added Layer: {0}", layer->GetName());
		layer->Initialise();

		for (auto& go : l.gameObjects)
		{
			auto gameObject = layer->AddGameObject(std::make_unique<GameObject>(go.name));
			gameObject->uid = go.uid;
			OSR_TRACE("   -> Added GameObject: {0}", gameObject->name);

			// TODO - Only supports Transform 2D
			gameObject->transform = std::move(std::make_unique<Transform2DComponent>(gameObject));
			Transform2DComponent* transform2DComponent = dynamic_cast<Transform2DComponent*>(gameObject->transform.get());
			transform2DComponent->SetPosition({ go.transformDef.transform2D.x, go.transformDef.transform2D.y });
			transform2DComponent->SetRotation(go.transformDef.transform2D.angle);
			transform2DComponent->SetScale({ go.transformDef.transform2D.scaleX, go.transformDef.transform2D.scaleY });

			for (auto& c : go.components)
			{
				OSR_TRACE("       -> Added Component: {0}", c.type);
				switch ((Osiris::SceneComponentType)c.type)
				{
				case Osiris::SceneComponentType::SpriteRenderer:
					{
						IBaseComponent* component = gameObject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::SpriteRenderer, gameObject)));
						Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)component;
						component->Initialise();
						spriteComponent->SetPosition(c.componentDef.sprite.x, c.componentDef.sprite.y);
						spriteComponent->SetSize(c.componentDef.sprite.width, c.componentDef.sprite.height);
					}
					break;
				case Osiris::SceneComponentType::CameraComponent:
					{
						IBaseComponent* component = gameObject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::CameraComponent, gameObject)));
						Osiris::CameraComponent* cameraComponent = (Osiris::CameraComponent*)component;
						component->Initialise();
						cameraComponent->SetCameraUID(c.componentDef.camera.uid);
						cameraComponent->GetCamera().SetSize(c.componentDef.camera.size);
						cameraComponent->SetSize(c.componentDef.camera.size);
						cameraComponent->GetCamera().SetViewportSize(800.0f, 600.00f);
					}
					break;
				}
			}
		}
	}

	scene.close();

	return std::move(newScene);
}