#pragma once
#include "osrpch.h"

/* Core includes */
#include "core/renderer/Renderer.h"
#include "core/renderer/Shader.h"

/* Local includes */
#include "Layer2D.h"

namespace Osiris::Editor
{
	Layer2D::Layer2D() : name("Untitled") { }

	Layer2D::Layer2D(const std::string& name) : name(name) { }

	Layer2D::Layer2D(const Layer2D& obj) : name(obj.name), gameobjects(obj.gameobjects) { }

	Layer2D::~Layer2D() { }

	void Layer2D::AddSprite(std::shared_ptr<GameObject> gameObject)
	{
		gameobjects.push_back(gameObject);
	}

	void Layer2D::RemoveSprite(int idx)
	{
		gameobjects.erase(gameobjects.begin() + idx);
	}

	void Layer2D::SwapSprite(int a, int b)
	{
		std::shared_ptr<GameObject> tmp = gameobjects[a];
		gameobjects[a] = gameobjects[b];
		gameobjects[b] = tmp;
	}
}