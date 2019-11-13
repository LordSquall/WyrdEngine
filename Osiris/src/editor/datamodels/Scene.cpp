#pragma once
#include "osrpch.h"

#include "Scene.h"
#include "GameObject.h"
#include "layers/Renderer2DLayer.h"

namespace Osiris::Editor
{
	Scene::Scene() : name("Untitled Scene") { }

	Scene::Scene(std::string name) : name(name) { }

	void Scene::BuildDefaults()
	{
		Layer2D layer0("Layer 2D - 0");
		layer0.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 0 - 0" })));
		layer0.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 0 - 1" })));
		layer0.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 0 - 2" })));
		layer0.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 0 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer0));

		Layer2D layer1("Layer 2D - 1");
		layer1.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 1 - 0" })));
		layer1.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 1 - 1" })));
		layer1.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 1 - 2" })));
		layer1.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 1 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer1));

		Layer2D layer2("Layer 2D - 2");
		layer2.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 2 - 0" })));
		layer2.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 2 - 1" })));
		layer2.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 2 - 2" })));
		layer2.gameObjects.push_back(std::make_shared<GameObject>(GameObject({ "Sprite 2 - 3" })));
		layers2D.push_back(std::make_shared<Layer2D>(layer2));
	}


	void Layer2D::AddSprite(const GameObject& gameObject)
	{
		gameObjects.push_back(std::make_shared<GameObject>(gameObject));

		//batch.AddSprite(new Sprite("Temp", 0, 0, 100, 100));
	}

	void Layer2D::RemoveSprite(int idx)
	{
		gameObjects.erase(gameObjects.begin() + idx);
	}

	void Layer2D::SwapSprite(int a, int b)
	{
		std::shared_ptr<GameObject> tmp = gameObjects[a];
		gameObjects[a] = gameObjects[b];
		gameObjects[b] = tmp;
	}
}