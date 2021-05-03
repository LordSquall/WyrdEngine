#pragma once

#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Texture.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/SceneComponentFactory.h>
#include <core/behaviour/ScriptedClass.h>

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
		WYRD_CORE_ERROR("Unable to find core.dat file");
		return;
	}

	/* core file configuration */
	CoreFormat_Config config;
	Read(core, config);

	WYRD_TRACE("Core Config Version: {0}", config.version);

	core.close();
}

void ImportManager::ImportGame(const std::string& root)
{
	/* open the file */
	std::ifstream game;
	game.open(root + "Game.dat", std::ios::in | std::ios::binary);
	if (game.good() == false)
	{
		WYRD_CORE_ERROR("Unable to find game.dat file");
		return;
	}

	/* game file configuration */
	GameFormat_Config config;
	Read(game, config);

	WYRD_TRACE("Game Config Version: {0}", config.version);
	
	Wyrd::Application::Get().GetWindow().SetTitle(config.name + " - " + config.version);

	///* display configuration */
	GameFormat_Display display;
	Read(game, display);

	Wyrd::Application::Get().GetWindow().SetSize(display.width, display.height);

	Application::Get().GetRenderer().SetViewport(
		(float)Application::Get().GetWindow().GetWidth(),
		(float)Application::Get().GetWindow().GetHeight()
	);

	/* scene listings */
	GameFormat_SceneListings sceneListings;
	Read(game, sceneListings);

	for (auto& listing : sceneListings.sceneUIDs) WYRD_TRACE("Scene Listing: {0}", listing);

	WYRD_TRACE("Initial Scene: {0}", sceneListings.initialScene);
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
		WYRD_CORE_ERROR("Unable to find common.bundle file");
		return;
	}

	/* common file configuration */
	BundleFormat_Config config;
	Read(common, config);

	WYRD_TRACE("Common Config Version: {0}", config.version);

	/* shader configuration */
	BundleFormat_ShaderConfig shaderConfig;
	Read(common, shaderConfig);

	WYRD_TRACE("Shader Config Count: {0}", shaderConfig.shaders_cnt);

	for (auto& s : shaderConfig.shaders)
	{
		std::shared_ptr<Wyrd::Shader> shader = std::shared_ptr<Wyrd::Shader>(Wyrd::Shader::Create());
		if (shader->Build(s.vertexSrc, s.fragmentSrc))
		{
			Wyrd::Application::Get().GetResources().Shaders[s.uid.str()] = shader;
			WYRD_INFO("Shader Compiled and Added! [{0}]", s.uid.str());
		}
		else
		{
			WYRD_ERROR("Unable to Compile Shader [{0}]", s.uid.str());
		}
	}

	/* texture configuration */
	BundleFormat_TextureConfig textureConfig;
	Read(common, textureConfig);

	WYRD_TRACE("Texture Config Count: {0}", textureConfig.textures_cnt);

	for (auto& t : textureConfig.textures)
	{
		std::shared_ptr<Wyrd::Texture> texture = std::shared_ptr<Wyrd::Texture>(Wyrd::Texture::Create((unsigned char*)&t.data[0], t.width, t.height, t.channels, "Debug Description"));
		texture->SetUID(t.uid);
		Application::Get().GetResources().Textures[t.uid] = texture;

		WYRD_INFO("Texture Added [{0}]", t.uid.str());
	}

	/* managed libaries configuration */
	BundleFormat_ManagedLibConfig managedLibConfig;
	Read(common, managedLibConfig);

	WYRD_TRACE("Managed Lib Config Lin Count		: {0}", managedLibConfig.managedLibraries_cnt);
	WYRD_TRACE("Managed Lib Config Classes Cnt   : {0}", managedLibConfig.classes_cnt);

	Application::Get().GetBehaviour().LoadBehaviourModel(managedLibConfig.classes, managedLibConfig.managedLibraries[0].data, managedLibConfig.managedLibraries[1].data);

	for (size_t i = 0; i < managedLibConfig.classes.size(); i++)
	{
		Application::Get().GetBehaviour().GetCustomClass(managedLibConfig.classes[i])->SetUID(managedLibConfig.classesUIDs[i]);
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

	/* common file configuration */
	SceneFormat_Config config;
	Read(scene, config);

	WYRD_TRACE("Scene Config Version: {0} Name: {1}", config.version, config.name);

	/* create new scene object to begin build */
	std::unique_ptr<Wyrd::Scene> newScene = std::make_unique<Wyrd::Scene>();

	/* primary camera */
	newScene->SetPrimaryCameraUID(config.camera);

	/* Layer configuration */
	SceneFormat_LayerConfig layerConfig;
	Read(scene, layerConfig);

	for (auto& l : layerConfig.layers)
	{
		auto& layer = newScene->AddLayer(std::make_unique<SceneLayer2D>(l.name));
		WYRD_TRACE(" -> Added Layer: {0}", layer->GetName());
		layer->Initialise();

		for (auto& go : l.gameObjects)
		{
			auto gameObject = layer->AddGameObject(std::make_unique<GameObject>(go.name));
			gameObject->uid = go.uid;
			WYRD_TRACE("   -> Added GameObject: {0}", gameObject->name);

			// TODO - Only supports Transform 2D
			gameObject->transform = std::move(std::make_unique<Transform2DComponent>(gameObject));
			Transform2DComponent* transform2DComponent = dynamic_cast<Transform2DComponent*>(gameObject->transform.get());
			transform2DComponent->SetPosition({ go.transformDef.transform2D.x, go.transformDef.transform2D.y });
			transform2DComponent->SetRotation(go.transformDef.transform2D.angle);
			transform2DComponent->SetScale({ go.transformDef.transform2D.scaleX, go.transformDef.transform2D.scaleY });

			for (auto& c : go.components)
			{
				switch ((Wyrd::SceneComponentType)c.type)
				{
				case Wyrd::SceneComponentType::SpriteRenderer:
					{
						WYRD_TRACE("       -> Added Component: Sprite Component");
						IBaseComponent* component = gameObject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::SpriteRenderer, gameObject)));
						Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)component;
						component->Initialise();
						spriteComponent->SetPosition(c.componentDef.sprite.x, c.componentDef.sprite.y);
						spriteComponent->SetSize(c.componentDef.sprite.width, c.componentDef.sprite.height);
						spriteComponent->SetTexture(Application::Get().GetResources().Textures[c.componentDef.sprite.texture]);

						WYRD_TRACE("           -> X:{0}, Y:{1}, W:{2}, H:{3}, UID:{4}", c.componentDef.sprite.x, c.componentDef.sprite.y, c.componentDef.sprite.width, c.componentDef.sprite.height, c.componentDef.sprite.texture);
					}
					break;
				case Wyrd::SceneComponentType::ScriptComponent:
					{
						WYRD_TRACE("       -> Added Component: Script Component");
						IBaseComponent* component = gameObject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::ScriptComponent, gameObject)));
						Wyrd::ScriptComponent* scriptComponent = (Wyrd::ScriptComponent*)component;
						component->Initialise();
						scriptComponent->SetClass(Application::Get().GetBehaviour().GetCustomClassByUID(c.componentDef.script.script));
						WYRD_TRACE("           -> Script UID:{0}", c.componentDef.script.script);
					}
					break;
				case Wyrd::SceneComponentType::CameraComponent:
					{
						WYRD_TRACE("       -> Added Component: Camera Component");
						IBaseComponent* component = gameObject->AddComponent(std::move(SceneComponentFactory::Create(SceneComponentType::CameraComponent, gameObject)));
						Wyrd::CameraComponent* cameraComponent = (Wyrd::CameraComponent*)component;
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


	newScene->AssignScripts(Application::Get().GetBehaviourPtr());

	return std::move(newScene);
}