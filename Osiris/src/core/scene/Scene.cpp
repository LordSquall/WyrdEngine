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
		
	}
}