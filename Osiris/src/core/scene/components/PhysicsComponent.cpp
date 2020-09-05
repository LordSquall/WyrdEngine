#pragma once

/* local includes */
#include "osrpch.h"
#include "PhysicsComponent.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/SpriteComponent.h"

namespace Osiris
{
	PhysicsComponent::PhysicsComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::PhysicsComponent) { }

	PhysicsComponent::PhysicsComponent(const PhysicsComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::PhysicsComponent) { }

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::Update(Timestep ts)
	{
		/* Update AABB global position */
		glm::vec4 spritePos = Owner->transform2D->matrix * glm::vec4(Owner->inputArea.x, Owner->inputArea.y, 0.0f, 1.0f);
		
		_AABB = { { spritePos.x, spritePos.y }, { Owner->inputArea.z, Owner->inputArea.w } };
	}
}