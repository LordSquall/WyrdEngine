/* local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "Physics.h"
#include "core/Application.h"
#include "core/scene/Scene.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/MonoUtils.h"

#include <glm/glm.hpp>

namespace Wyrd
{
	struct CollisionHit
	{
		Wyrd::PhysicsComponent* a;
		Wyrd::PhysicsComponent* b;
		Ray::Hit* hit;
	};

	Physics::Physics() : _IsRunning(false)
	{
	}
	
	Physics::~Physics()
	{

	}

	void Physics::Start(std::shared_ptr<Scene> scene)
	{
		_IsRunning = true;
	}

	void Physics::Update(Timestep ts)
	{
	}

	void Physics::Reset()
	{
		WYRD_CORE_TRACE("Physics::Reset() called!");
	}

	void Physics::Stop()
	{
		WYRD_CORE_TRACE("Physics::Stop() called!");

		_physicsObjects.clear();

		_IsRunning = false;
	}

	void Physics::AddObject(PhysicsComponent* object)
	{
		_physicsObjects.push_back(object);
	}


	void Physics::RemoveObject(PhysicsComponent* object)
	{
		
	}
}
