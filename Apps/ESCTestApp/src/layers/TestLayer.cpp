#pragma once

#include "TestLayer.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/CameraComponent.h>
#include <core/scene/components/Transform2DComponent.h>

#include <core/ecs/ECSEntity.h>

#include <fstream>

bool TestLayer::OnAttach()
{
	ECSEntity entity = CreateEntity();

	WYRD_TRACE("Entity id: {0}", entity);
	return true;
}

void TestLayer::OnDetach() { }

void TestLayer::OnUpdate(Timestep ts) { }

void TestLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	renderer.Clear(0.7, 0.3, 0.3);
}