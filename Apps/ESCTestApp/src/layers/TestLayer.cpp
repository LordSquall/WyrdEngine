#pragma once


#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/scene/SceneLayer2D.h>
#include <core/scene/components/CameraComponent.h>
#include <core/scene/components/Transform2DComponent.h>

#include <fstream>

#include "TestLayer.h"

struct MovementComponent
{
	float x;
	float y;
};

struct PhysicsUpdateComponent
{
	float dx;
	float dy;
};


bool TestLayer::OnAttach()
{
	//Entity entity = scene.CreateEntity();
	//
	//MovementComponent* movementComp = scene.AssignComponent<MovementComponent>(entity);
	//PhysicsUpdateComponent* physicsComp = scene.AssignComponent<PhysicsUpdateComponent>(entity);
	//
	//physicsComp->dx = 0.1f;
	//physicsComp->dy = 0.2f;

	return true;
}

void TestLayer::OnDetach() { }

void TestLayer::OnUpdate(Timestep ts) 
{
	//// Update position based on physics delta
	//for (Entity e : ComponentSet<MovementComponent, PhysicsUpdateComponent>(scene))
	//{
	//	MovementComponent*		movementComp	= scene.Get<MovementComponent>(e);
	//	PhysicsUpdateComponent* physicsComp		= scene.Get<PhysicsUpdateComponent>(e);
	//	
	//	movementComp->x += physicsComp->dx;
	//	movementComp->y += physicsComp->dy;
	//}
	//
	//// print to screen
	//for (Entity e : ComponentSet<MovementComponent>(scene))
	//{
	//	MovementComponent* movementComp = scene.Get<MovementComponent>(e);
	//
	//	WYRD_TRACE("MovementComp: {0}, {1}", movementComp->x, movementComp->y);
	//}
}

void TestLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	renderer.Clear(0.7, 0.3, 0.3);
}