#pragma once

/* local includes */
#include "wyrdpch.h"
#include "PhysicsComponent.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/SpriteComponent.h"

#include <jsonxx.h>

namespace Wyrd
{
	PhysicsComponent::PhysicsComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{ 		
		Initialised = true;
	}

	PhysicsComponent::PhysicsComponent(const PhysicsComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::PhysicsComponent),
		_velocity({ 0.0f, 0.0 })
	{
		Initialised = true;
	}

	PhysicsComponent::~PhysicsComponent()
	{

	}


	void PhysicsComponent::Recalculate()
	{
		if (_UseSpriteBoundary)
		{
			if (Owner->sprite != nullptr)
			{
				//_AABB = { Owner->transform2D->globalPosition + Owner->sprite->position, Owner->sprite->size };
			}
			else
			{
				//TODO using print instead of OSR_
				//printf("Unable to Add Physics Component with 'UseSpriteBoundary' without a matching Sprite Component");
			}
		}
	}

	void PhysicsComponent::Update(Timestep ts)
	{
		/* Apply physics position update */
		//Owner->transform2D->position += _velocity * ts.GetSeconds();
		//Owner->transform2D->SetMatrixValid(false);
	}

	void PhysicsComponent::AddCollisionState(PhysicsComponent* collisionKey)
	{
		if (_CollisionStates.find(collisionKey) == _CollisionStates.end())
		{
			_CollisionStates.insert({ collisionKey, 1 });
			//Application::Get().GetBehaviour().BroadcastTriggerCall(Owner, std::string("OnTriggerCollision"), collisionKey->Owner, { });
			
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
		}
	}

	bool PhysicsComponent::ToJson(jsonxx::Object& object)
	{
		object << "componentType" << (int)SceneComponentType::PhysicsComponent;

		/* is static */
		object << "isStatic" << _IsStatic;

		/* is trigger */
		object << "isTrigger" << _IsTrigger;

		/* use sprite boundary*/
		object << "useSpriteBoundary" << _UseSpriteBoundary;

		return true;
	}

	bool PhysicsComponent::FromJson(jsonxx::Object& object)
	{
		/* is static */
		_IsStatic = object.get<jsonxx::Boolean>("isStatic", false);

		/* is trigger */
		_IsTrigger = object.get<jsonxx::Boolean>("isTrigger", false);

		/* use sprite boundary*/
		_UseSpriteBoundary = object.get<jsonxx::Boolean>("useSpriteBoundary", false);

		return true;
	}
}