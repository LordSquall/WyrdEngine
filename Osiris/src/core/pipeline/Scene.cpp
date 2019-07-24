#pragma once
#include "osrpch.h"

#include "Scene.h"
#include "layers/Renderer2DLayer.h"

namespace Osiris
{
	uint32_t Scene::_nextID = 0;


	void Scene::Unload()
	{
		Layers::Renderer2DLayer* renderer2DLayer = (Layers::Renderer2DLayer*)Application::Get().GetLayerStack()->FindLayer("2D Rendering Layer");

		renderer2DLayer->RemoveAllSpriteLayers();
	}
}