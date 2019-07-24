#pragma once
#include "osrpch.h"

#include "SceneLoader.h"

#include "layers/Renderer2DLayer.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris
{
	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene, FileContent content)
	{
		std::ifstream i(path);
		json j;
		i >> j;

		/* clear the current scene */
		scene.Unload();

		scene.SetName(j["name"]);

		Layers::Renderer2DLayer* renderer2DLayer = (Layers::Renderer2DLayer*)Application::Get().GetLayerStack()->FindLayer("2D Rendering Layer");


		// the same code as range for
		for (auto& spriteLayer : j["spritelayers"].items()) {
			SpriteLayer* layer = renderer2DLayer->AddSpriteLayer(spriteLayer.key());

			for (auto& sprite : spriteLayer.value().items()) {
				layer->AddSprite(new Sprite(
					sprite.value()["name"],
					sprite.value()["xpos"],
					sprite.value()["ypos"],
					sprite.value()["width"],
					sprite.value()["height"]
				));
			}
		}

		return Success;
	}

	SceneLoader::Result SceneLoader::Save(std::string path, Scene& scene, FileContent content)
	{
		json j;
		j["name"] = "test";

		Layers::Renderer2DLayer* renderer2DLayer = (Layers::Renderer2DLayer*)Application::Get().GetLayerStack()->FindLayer("2D Rendering Layer");

		for (auto& spriteLayer : renderer2DLayer->GetSpriteLayers())
		{
			for (auto& sprite : spriteLayer->GetSprites())
			{
				j["spritelayers"][spriteLayer->GetName()][sprite->GetID()]["name"] = sprite->GetName();
				j["spritelayers"][spriteLayer->GetName()][sprite->GetID()]["xpos"] = sprite->GetX();
				j["spritelayers"][spriteLayer->GetName()][sprite->GetID()]["ypos"] = sprite->GetY();
				j["spritelayers"][spriteLayer->GetName()][sprite->GetID()]["width"] = sprite->GetWidth();
				j["spritelayers"][spriteLayer->GetName()][sprite->GetID()]["height"] = sprite->GetHeight();
			}
		}

		std::ofstream o(path);

		o << std::setw(4) << j << std::endl;

		o.close();

		return Success;
	}
}