#pragma once
#include "osrpch.h"

/* Local includes */
#include "Scene.h"
#include "Layer2D.h"
#include "GameObject.h"
#include "core/scene/components/Transform2DComponent.h"

namespace Osiris
{
	Scene::Scene() : name("Untitled Scene") 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}

	Scene::Scene(std::string name) : name(name) 
	{
		bgcolor.r = 0.2f;
		bgcolor.g = 0.2f;
		bgcolor.b = 0.2f;
		bgcolor.a = 1.0f;
	}


	void Scene::Update()
	{
		/* update each layer */
		for(auto layer : layers2D)
		{
			layer->Update();
		}
	}


	std::shared_ptr<GameObject> Scene::FindGameObject(const UID uid)
	{
		/* update each layer */
		for (auto& layer : layers2D)
		{
			for (auto& gameObject : layer->children)
			{
				if (gameObject->uid == uid)
					return gameObject;

				auto found = gameObject->FindChild(uid);
				if (found != nullptr)
					return found;
			}
		}

		return nullptr;
	}


	void Scene::AssignScripts(Behaviour* behaviour)
	{
		for (auto& layer : layers2D)
		{
			layer->AssignScripts(behaviour);
		}
	}
}