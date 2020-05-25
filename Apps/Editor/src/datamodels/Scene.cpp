#pragma once
#include "osrpch.h"

/* Local includes */
#include "Scene.h"
#include "GameObject.h"

namespace Osiris::Editor
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
}