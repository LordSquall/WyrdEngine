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
	PhysicsComponent::PhysicsComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{ }

	PhysicsComponent::PhysicsComponent(const PhysicsComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{ }

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::Update(Timestep ts)
	{
		/* Apply physics position update */
		Owner->transform2D->position += _velocity * ts.GetSeconds();
		Owner->transform2D->SetMatrixValid(false);
		
		/* Update AABB global position */
		glm::vec4 spritePos = Owner->transform2D->matrix * glm::vec4(Owner->inputArea.x, Owner->inputArea.y, 0.0f, 1.0f);
		
		_AABB = { { spritePos.x, spritePos.y }, { Owner->inputArea.z, Owner->inputArea.w } };
	}
}