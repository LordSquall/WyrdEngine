#pragma once

/* local includes */
#include "osrpch.h"
#include "PhysicsComponent.h"
#include "core/scene/components/PhysicsComponent.h"

namespace Osiris
{
	PhysicsComponent::PhysicsComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::PhysicsComponent)
	{
		//OwnerGameObject = owner;
	}

	PhysicsComponent::PhysicsComponent(const PhysicsComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::PhysicsComponent)
	{
		//OwnerGameObject = obj.OwnerGameObject;
	}

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::Update()
	{
		/* Update AABB global position */
		//glm::vec4 spritePos = OwnerGameObject->transform2d.matrix * glm::vec4(OwnerGameObject->spriteRender.Sprite->GetX(), OwnerGameObject->spriteRender.Sprite->GetY(), 0.0f, 1.0f);
		
		//_AABB = { spritePos.x, spritePos.y, OwnerGameObject->spriteRender.Sprite->GetWidth(), OwnerGameObject->spriteRender.Sprite->GetHeight() };

		//_Polygon.BuildFromRectangle(_AABB);
	}
}