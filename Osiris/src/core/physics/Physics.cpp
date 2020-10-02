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
	struct CollisionHit
	{
		Osiris::PhysicsComponent* a;
		Osiris::PhysicsComponent* b;
		Ray::Hit& hit;
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
		std::vector<CollisionHit> collisions;

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
				for (auto physicsB : _physicsObjects)
				{
					/* we don't want to evaluate self colisions */
					if (physicsA->Owner->GetUID() != physicsB->Owner->GetUID())
					{
						if (physicsA->IsTrigger() == false && (physicsB->IsTrigger() == false && physicsB->IsStatic() == true))
						{	
							// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
							if (physicsA->GetVelocity().x != 0 || physicsA->GetVelocity().y != 0)
							{
								// Expand target rectangle by source dimensions
								Rect expanded_target;
								expanded_target.position = physicsB->GetAABB().position - physicsA->GetAABB().size / 2.0f;
								expanded_target.size = physicsB->GetAABB().size + physicsA->GetAABB().size;

								// Create Ray
								Ray ray = { 
									{ physicsA->GetAABB().position + physicsA->GetAABB().size / 2.0f },
									{ physicsA->GetVelocity() * ts.GetSeconds() } 
								};

								Ray::Hit hit;

								if (expanded_target.ContainsRay(ray, hit) == true)
								{
									if ((hit.distance >= 0.0f && hit.distance < 1.0f))
									{
										OSR_TRACE("Detected");
										collisions.push_back({ physicsA, physicsB, hit });
									}
								}
							}
						}
					}
				}
			}

			// Sort collisions based on distance
			/*std::sort(collisions.begin(), collisions.end(), [](const CollisionHit& a, const CollisionHit& b)
				{
					return a.hit.distance < b.hit.distance;
				});*/

		/*	for each (auto collision in collisions)
			{
				if (collision.hit.normal.y > 0) collision.a->GetAABB().contact[0] = &collision.b->GetAABB(); else nullptr;
				if (collision.hit.normal.x < 0) collision.a->GetAABB().contact[1] = &collision.b->GetAABB(); else nullptr;
				if (collision.hit.normal.y < 0) collision.a->GetAABB().contact[2] = &collision.b->GetAABB(); else nullptr;
				if (collision.hit.normal.x > 0) collision.a->GetAABB().contact[3] = &collision.b->GetAABB(); else nullptr;

				collision.a->SetVelocity(collision.a->GetVelocity() + collision.hit.normal * glm::vec2(std::abs(collision.a->GetVelocity().x), std::abs(collision.a->GetVelocity().y)) * (1 - collision.hit.distance));
			}*/
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
