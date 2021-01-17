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
		bgcolor[0] = 0.2f;
		bgcolor[1] = 0.2f;
		bgcolor[2] = 0.2f;
	}

	Scene::Scene(std::string name) : name(name) 
	{
		bgcolor[0] = 0.2f;
		bgcolor[1] = 0.2f;
		bgcolor[2] = 0.2f;
	}


	void Scene::Update()
	{
		/* update each layer */
		for(auto layer : layers2D)
		{
			layer->Update();
		}
	}


	std::shared_ptr<GameObject> Scene::FindGameObject(const uint32_t uid)
	{
		/* update each layer */
		for (auto& layer : layers2D)
		{
			for (auto& gameObject : layer->children)
			{
				if (gameObject->GetUID() == uid)
					return gameObject;

				auto found = gameObject->FindChild(uid);
				if (found != nullptr)
					return found;
			}
		}

		return nullptr;
	}
}