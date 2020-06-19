#pragma once
#include "osrpch.h"

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "support/JsonExtensions.hpp"

#include "SceneLoader.h"

#include "core/Application.h"

#include "services/ServiceManager.h"
#include "services/ResourceService.h"

#include "layers/Renderer2DLayer.h"
#include "datamodels/components/Transform2DComponent.h"
#include "datamodels/components/Transform3DComponent.h"
#include "datamodels/components/SpriteComponent.h"

using namespace nlohmann;
using namespace glm;

namespace Osiris::Editor
{
	/* Json operation overloader functions */
	void to_json(json& jScene, const Scene& scene);
	void from_json(const json& jScene, Scene& scene);
	void to_json(json& jLayer2d, const Layer2D& layer2d);
	void from_json(const json& jLayer2d, Layer2D& layer2d);
	void to_json(json& jGameObject2D, const GameObject& gameObject2d);
	void from_json(const json& jGameObject2D, GameObject& gameObject2d);

	void to_json(json& jTransform2d, const Transform2DComponent& transform2d);
	void from_json(const json& jTransform2d, Transform2DComponent& transform2d);
	void to_json(json& jTransform3d, const Transform3DComponent& transform3d);
	void from_json(const json& jTransform3d, Transform3DComponent& transform3d);

	void to_json(json& jSpriteComponent, const SpriteComponent& spriteComponent);
	void from_json(const json& jSpriteComponent, SpriteComponent& spriteComponent);


	void to_json(json& jScriptComponent, const ScriptComponent& scriptComponent);
	void from_json(const json& jScriptComponent, ScriptComponent& scriptComponent);


	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene, FileContent content)
	{
		json j;
		std::ifstream i(path);

		if (i.is_open())
		{
			i >> j;
			scene = j.get<Scene>();

			/* there are a number of operations that now need to be performed on the model before continuing */
			for each (auto& layer in scene.layers2D)
			{
				for each (auto& go in layer->gameobjects)
				{
					go->spriteRender.OwnerGameObject = go;
					go->script.OwnerGameObject = go;
					go->transform2d.OwnerGameObject = go;
				}
			}

			return Success;
		}
		return FileNotFound;
	}

	SceneLoader::Result SceneLoader::Save(std::string path, Scene& scene, FileContent content)
	{
		json j = scene;

		std::ofstream o(path);

		o << std::setw(4) << j << std::endl;

		o.close();

		return Success;
	}

	void to_json(json& jScene, const Scene& scene) {
		jScene = json::object();

		float* color = &Application::Get().color[0];

		/* store scene wide parameters */
		jScene["name"] = scene.name;
		jScene["bgcolor"] = { color[0], color[1], color[2] };
		jScene["layers2D"] = scene.layers2D;

		/* store editor camera settings */
		jScene["cameraPosition"] = { scene.cameraPosition.x, scene.cameraPosition.y };
		jScene["cameraZoom"] = scene.cameraZoom;
	}

	void from_json(const json& jScene, Scene& scene) {
		jScene.at("name").get_to(scene.name);
		jScene.at("bgcolor")[0].get_to(scene.bgcolor[0]);
		jScene.at("bgcolor")[1].get_to(scene.bgcolor[1]);
		jScene.at("bgcolor")[2].get_to(scene.bgcolor[2]);
		jScene.at("layers2D").get_to(scene.layers2D);

		jScene.at("cameraPosition")[0].get_to(scene.cameraPosition.x);
		jScene.at("cameraPosition")[1].get_to(scene.cameraPosition.y);
		jScene.at("cameraZoom").get_to(scene.cameraZoom);
	}

	void to_json(json& jLayer2d, const Layer2D& layer2d) {
		jLayer2d = json::object();
		jLayer2d["name"] = layer2d.name;
		jLayer2d["gameObjects"] = layer2d.gameobjects;
	}

	void from_json(const json& jLayer2d, Layer2D& layer2d)
	{
		jLayer2d.at("name").get_to(layer2d.name);
		jLayer2d.at("gameObjects").get_to(layer2d.gameobjects);
	}

	void to_json(json& jGameObject, const GameObject& gameObject) {
		jGameObject = json::object();
		jGameObject["name"] = gameObject.name;
		jGameObject["transform2d"] = gameObject.transform2d;
		jGameObject["spriteRenderer"] = gameObject.spriteRender;
		jGameObject["script"] = gameObject.script;
	}

	void from_json(const json& jGameObject, GameObject& gameObject) {
		jGameObject.at("name").get_to(gameObject.name);
		jGameObject.at("transform2d").get_to(gameObject.transform2d);
		jGameObject.at("spriteRenderer").get_to(gameObject.spriteRender);

		if(jGameObject.find("script") != jGameObject.end())
			jGameObject.at("script").get_to(gameObject.script);

		gameObject.inputArea.x = (float)gameObject.spriteRender.Sprite->GetX();
		gameObject.inputArea.y = (float)gameObject.spriteRender.Sprite->GetY();
		gameObject.inputArea.z = (float)gameObject.spriteRender.Sprite->GetWidth();
		gameObject.inputArea.w = (float)gameObject.spriteRender.Sprite->GetHeight();
	}

	void to_json(json& jTransform, const Transform2DComponent& transform2d)
	{
		jTransform["position"] = { transform2d.position.x, transform2d.position.y };
		jTransform["rotation"] = { transform2d.rotation };
		jTransform["scale"] = { transform2d.scale.x, transform2d.scale.y };
	}

	void from_json(const json& jTransform, Transform2DComponent& transform2d)
	{
		jTransform.at("position")[0].get_to(transform2d.position.x);
		jTransform.at("position")[1].get_to(transform2d.position.y);
		jTransform.at("rotation")[0].get_to(transform2d.rotation);
		jTransform.at("scale")[0].get_to(transform2d.scale.x);
		jTransform.at("scale")[1].get_to(transform2d.scale.y);
		
		transform2d.UpdateModelMatrix();
	}

	void to_json(json& jTransform3d, const Transform3DComponent& transform3d)
	{
		jTransform3d["position"] = { transform3d.position.x, transform3d.position.y, transform3d.position.z };
		jTransform3d["rotation"] = { transform3d.rotation.x, transform3d.rotation.y, transform3d.rotation.y };
		jTransform3d["scale"] = { transform3d.scale.x, transform3d.scale.y, transform3d.scale.y };
	}

	void from_json(const json& jTransform3d, Transform3DComponent& transform3d)
	{
		jTransform3d.at("position")[0].get_to(transform3d.position.x);
		jTransform3d.at("position")[1].get_to(transform3d.position.y);
		jTransform3d.at("position")[2].get_to(transform3d.position.y);
		jTransform3d.at("rotation")[0].get_to(transform3d.rotation.x);
		jTransform3d.at("rotation")[1].get_to(transform3d.rotation.y);
		jTransform3d.at("rotation")[2].get_to(transform3d.rotation.z);
		jTransform3d.at("scale")[0].get_to(transform3d.scale.x);
		jTransform3d.at("scale")[1].get_to(transform3d.scale.y);
		jTransform3d.at("scale")[2].get_to(transform3d.scale.y);
	}

	void to_json(json& jSpriteComponent, const SpriteComponent& spriteComponent)
	{
		jSpriteComponent["position"] = { spriteComponent.Sprite->GetX(), spriteComponent.Sprite->GetY() };
		jSpriteComponent["dimensions"] = { spriteComponent.Sprite->GetWidth(), spriteComponent.Sprite->GetHeight() };
		jSpriteComponent["baseTexture"] = spriteComponent.BaseTexture->GetName();
		jSpriteComponent["color"] = { spriteComponent.Color.r, spriteComponent.Color.g, spriteComponent.Color.b };
	}

	void from_json(const json& jSpriteComponent, SpriteComponent& spriteComponent)
	{
		vec2 pos = {
			jSpriteComponent.at("position")[0],
			jSpriteComponent.at("position")[1]
		};

		vec2 dim = {
			jSpriteComponent.at("dimensions")[0],
			jSpriteComponent.at("dimensions")[1]
		};
		
		vec3 color = {
			jSpriteComponent.at("color")[0],
			jSpriteComponent.at("color")[1],
			jSpriteComponent.at("color")[2],
		};

		std::string baseTextureName = jSpriteComponent.at("baseTexture");

		spriteComponent.Sprite = std::make_shared<Sprite>("Temp", (int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y);
		spriteComponent.BaseTexture = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetTextureByName(baseTextureName);
		spriteComponent.Color = color;

		spriteComponent.Sprite->SetTexture(spriteComponent.BaseTexture->GetTexture());
	}

	void to_json(json& jScriptComponent, const ScriptComponent& scriptComponent)
	{
		if(scriptComponent.GetScriptResource() != nullptr)
			jScriptComponent["scriptres"] = scriptComponent.GetScriptResource()->GetName();
		else
			jScriptComponent["scriptres"] = "";
	}

	void from_json(const json& jScriptComponent, ScriptComponent& scriptComponent)
	{
		if (jScriptComponent.find("scriptres") != jScriptComponent.end())
		{
			std::string baseScriptName = jScriptComponent.at("scriptres");
			scriptComponent.SetScriptResource(ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetScriptByName(baseScriptName));
		}
	}
}
