#pragma once

/* local includes */
#include "osrpch.h"
#include "PhysicsComponent.h"
#include "core/Log.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/SpriteComponent.h"

namespace Osiris
{
	PhysicsComponent::PhysicsComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{ 
		/* Update AABB global position */
		glm::vec4 spritePos = Owner->transform2D->matrix * glm::vec4(Owner->inputArea.x, Owner->inputArea.y, 0.0f, 1.0f);

		_AABB = { { spritePos.x, spritePos.y }, { Owner->inputArea.z, Owner->inputArea.w } };
	}

	PhysicsComponent::PhysicsComponent(const PhysicsComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{ 
		/* Update AABB global position */
		glm::vec4 spritePos = Owner->transform2D->matrix * glm::vec4(Owner->inputArea.x, Owner->inputArea.y, 0.0f, 1.0f);

		_AABB = { { spritePos.x, spritePos.y }, { Owner->inputArea.z, Owner->inputArea.w } };
	}

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

	void PhysicsComponent::AddCollisionState(PhysicsComponent* collisionKey)
	{
		if (_CollisionStates.find(collisionKey) == _CollisionStates.end())
		{
			_CollisionStates.insert({ collisionKey, 1 });
			OSR_TRACE("Collision State Added: {0} -> {1}", this->Owner->name, collisionKey->Owner->name);

			Application::Get().GetBehaviour().BroadcastTriggerCall(Owner, std::string("OnTriggerCollision"), collisionKey->Owner, { });
			Application::Get().GetBehaviour().BroadcastTriggerCall(collisionKey->Owner, std::string("OnTriggerCollision"), Owner, { });
		}
	}

	unsigned int PhysicsComponent::CurrentCollisionState(PhysicsComponent* collisionKey)
	{
		if(_CollisionStates.find(collisionKey) != _CollisionStates.end())
			return _CollisionStates[collisionKey];
		return 0;
	}

	void PhysicsComponent::RemoveCollisionState(PhysicsComponent* collisionKey)
	{

		if (_CollisionStates.find(collisionKey) != _CollisionStates.end())
		{
			_CollisionStates.erase(collisionKey);
			OSR_TRACE("Collision State Removed: {0} -> {1}", this->Owner->name, collisionKey->Owner->name);
		}
	}
}