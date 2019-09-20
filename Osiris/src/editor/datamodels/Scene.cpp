#pragma once
#include "osrpch.h"

#include "Scene.h"
#include "layers/Renderer2DLayer.h"

namespace Osiris::Editor
{
	Scene::Scene() : name("Untitled Scene") { }

	Scene::Scene(std::string name) : name(name) { }

	void Scene::BuildDefaults()
	{
		Layer2D layer0;
		layer0.name = "Layer 2D - 0";
		layer0.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 0 - 0" })));
		layer0.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 0 - 1" })));
		layer0.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 0 - 2" })));
		layer0.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 0 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer0));

		Layer2D layer1;
		layer1.name = "Layer 2D - 1";
		layer1.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 1 - 0" })));
		layer1.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 1 - 1" })));
		layer1.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 1 - 2" })));
		layer1.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 1 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer1));

		Layer2D layer2;
		layer2.name = "Layer 2D - 2";
		layer2.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 2 - 0" })));
		layer2.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 2 - 1" })));
		layer2.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 2 - 2" })));
		layer2.sprites.push_back(std::make_shared<GameObject2D>(GameObject2D({ "Sprite 2 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer2));
	}


	void Layer2D::AddSprite(const GameObject2D& gameObject)
	{
		sprites.push_back(std::make_shared<GameObject2D>(gameObject));
	}

	void Layer2D::RemoveSprite(int idx)
	{
		sprites.erase(sprites.begin() + idx);
	}

	void Layer2D::SwapSprite(int a, int b)
	{
		std::shared_ptr<GameObject2D> tmp = sprites[a];
		sprites[a] = sprites[b];
		sprites[b] = tmp;
	}
}