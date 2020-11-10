#pragma once
#include "osrpch.h"

/* Core includes */
#include "core/renderer/Renderer.h"
#include "core/renderer/Shader.h"

/* Local includes */
#include "Layer2D.h"

namespace Osiris
{
	Layer2D::Layer2D() : GameObject("Untitled") { }

	Layer2D::Layer2D(const std::string& name) : GameObject(name) { }

	Layer2D::Layer2D(const Layer2D& obj) : GameObject(obj.name) { }

	Layer2D::~Layer2D() { }

	void Layer2D::Update() { }

}