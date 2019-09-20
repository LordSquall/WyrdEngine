#pragma once
#include "osrpch.h"

#include "SceneLoader.h"

#include "layers/Renderer2DLayer.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris::Editor
{
	/* Json operation overloader functions */
	void to_json(json& jScene, const Scene& scene);
	void from_json(const json& jScene, Scene& scene);
	void to_json(json& jLayer2d, const Layer2D& layer2d);
	void from_json(const json& jLayer2d, Layer2D& layer2d);
	void to_json(json& jGameObject2D, const GameObject2D& gameObject2d);
	void from_json(const json& jGameObject2D, GameObject2D& gameObject2d);

	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene, FileContent content)
	{
		std::ifstream i(path);
		json j;
		i >> j;
	
		std::cout << j << std::endl;

		scene = j.get<Scene>();

		return Success;
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

		/* store scene wide parameters */
		jScene["name"] = scene.name;
		jScene["layers2D"] = scene.layers2D;
	}

	void from_json(const json& jScene, Scene& scene) {
		jScene.at("name").get_to(scene.name);
		jScene.at("layers2D").get_to(scene.layers2D);
	}

	void to_json(json& jLayer2d, const Layer2D& layer2d) {
		jLayer2d = json::object();
		jLayer2d["name"] = layer2d.name;
		jLayer2d["sprites"] = layer2d.sprites;
	}

	void from_json(const json& jLayer2d, Layer2D& layer2d) {
		jLayer2d.at("name").get_to(layer2d.name);
		jLayer2d.at("sprites").get_to(layer2d.sprites);
	}

	void to_json(json& jGameObject2d, const GameObject2D& gameObject2d) {
		jGameObject2d = json::object();
		jGameObject2d["name"] = gameObject2d.name;
	}

	void from_json(const json& jGameObject2d, GameObject2D& gameObject2d) {
		jGameObject2d.at("name").get_to(gameObject2d.name);

	}
}