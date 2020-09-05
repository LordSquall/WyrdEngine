/* local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "Physics.h"
#include "core/scene/Scene.h"
#include "core/scene/Layer2D.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/PhysicsComponent.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/MonoUtils.h"

namespace Osiris
{
	Physics::Physics() : _IsRunning(false)
	{
	}
	
	Physics::~Physics()
	{

	}

	void Physics::Start(std::shared_ptr<Scene> scene)
	{
		_IsRunning = true;

		// iterate the scene and find all objects with a physics component 
		for (auto& layer : scene->layers2D)
		{
			for (auto gameObject : layer->gameobjects)
			{
				for (auto component : gameObject->components)
				{
					if (component->GetType() == SceneComponentType::PhysicsComponent)
					{
						OSR_CORE_INFO("Found Physics Component on : {0}", gameObject->name);
						
						_physicsObjects.push_back((PhysicsComponent*)&*component);
					}
				}
			}
		}

	}

	void Physics::Update(Timestep ts)
	{
		if (_IsRunning == true)
		{
			/* update all the physics components */
			for (auto& component : _physicsObjects)
			{
				component->Update(ts);
			}

			for (auto& physicsA : _physicsObjects)
			{				
				/* start check against all other gameobject */
				for (auto& physicsB : _physicsObjects)
				{
					/* we don't want to evaluate self colisions */
					if (physicsA->Owner->GetUID() != physicsB->Owner->GetUID())
					{
						/* there are 4 combinations each with a different test */

						/* trigger on trigger */
						if (physicsA->IsTrigger() == true && physicsB->IsTrigger() == true)
						{
							// Not supported
						}
						/* trigger on non-trigger */
						else if (physicsA->IsTrigger() == true && physicsB->IsTrigger() == false)
						{
							
						}
						/* non-trigger on trigger */
						else if (physicsA->IsTrigger() == false && physicsB->IsTrigger() == true)
						{
							
						}
						/* non-trigger on non-trigger */
						else if (physicsA->IsTrigger() == false && physicsB->IsTrigger() == false)
						{
							
						}
					}
				}
			}
		}
	}

	void Physics::Reset()
	{
		OSR_CORE_TRACE("Physics::Reset() called!");
	}

	void Physics::Stop()
	{
		OSR_CORE_TRACE("Physics::Stop() called!");

		_IsRunning = false;
	}
}
