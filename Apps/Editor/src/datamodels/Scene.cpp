#pragma once
#include "osrpch.h"

/* Local includes */
#include "Scene.h"
#include "GameObject.h"

namespace Osiris::Editor
{
	Scene::Scene() : name("Untitled Scene") { }

	Scene::Scene(std::string name) : name(name) { }
}