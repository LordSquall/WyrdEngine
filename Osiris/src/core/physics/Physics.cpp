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

#include <glm/glm.hpp>

namespace Osiris
{
	struct CollisionHit
	{
		Osiris::PhysicsComponent* a;
		Osiris::PhysicsComponent* b;
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

		// iterate the scene and find all objects with a physics component 
		for (auto& layer : scene->layers2D)
		{
			for (auto gameObject : layer->children)
			{
				SearchGameObject(gameObject);
			}
		}
	}

	void Physics::Update(Timestep ts)
	{
		std::vector<CollisionHit> resolvableCollisions;

		if (_IsRunning == true)
		{
			for (auto &physicsA : _physicsObjects)
			{				
				/* start check against all other gameobject */
				for (auto &physicsB : _physicsObjects)
				{
					/* we don't want to evaluate self colisions */
					if (physicsA->Owner->GetUID() != physicsB->Owner->GetUID())
					{
						/* first of all, if the AABB is overlapping then we need to update the collision states */
						if (physicsA->GetAABB().ContainsRect(physicsB->GetAABB()))
						{
							physicsA->AddCollisionState(physicsB);
						}
						else
						{
							if (physicsA->GetVelocity().x != 0 || physicsA->GetVelocity().y != 0)
							{
								// Expand target rectangle by source dimensions
								Rect expanded_target;
								expanded_target.position = physicsB->GetAABB().position - physicsA->GetAABB().size / 2.0f;
								expanded_target.size = physicsB->GetAABB().size + physicsA->GetAABB().size;

								// Create Ray
								Ray ray = {
									{ physicsA->GetAABB().position + physicsA->GetAABB().size / 2.0f },
									{ glm::normalize(physicsA->GetVelocity()) }
								};

								Ray::Hit hit;
								bool shouldResolve = !physicsA->IsTrigger() && !physicsB->IsTrigger();

								if (expanded_target.ContainsRay(ray, hit) == true)
								{
									if ((hit.distance >= 0.0f && hit.distance < 1.0f))
									{
										/**
										* At this point the collision has been detected between the gameobjects,
										* however depending on the physics properties we either need to
										* trigger or resolve
										*/
										physicsA->AddCollisionState(physicsB);

										if (shouldResolve)
										{
											resolvableCollisions.push_back({ physicsA, physicsB, &hit });
										}
									}
									else
									{
										if (shouldResolve)
										{
											if (physicsA->CurrentCollisionState(physicsB) != 0)
											{
												physicsA->RemoveCollisionState(physicsB);
											}
										}
									}
								}
								else
								{
									if (shouldResolve)
									{
										if (physicsA->CurrentCollisionState(physicsB) != 0)
										{
											physicsA->RemoveCollisionState(physicsB);
										}
									}
								}
							}
							else
							{
								if (physicsA->CurrentCollisionState(physicsB) != 0)
								{
									physicsA->RemoveCollisionState(physicsB);
								}
							}
						}
					}
				}

				std::sort(resolvableCollisions.begin(), resolvableCollisions.end(), [](const CollisionHit& a, const CollisionHit& b)
					{
						return a.hit->distance < b.hit->distance;
					});

				for(auto collision : resolvableCollisions)
				{
					glm::vec2 currentVelocity = collision.a->GetVelocity();
					collision.a->SetVelocity(currentVelocity + (collision.hit->normal * glm::vec2(std::abs(currentVelocity.x), std::abs(currentVelocity.y)) * (1 - collision.hit->distance)));
				}
			}

			/* update all the physics components */
			for (auto& component : _physicsObjects)
			{
				component->Update(ts);
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

		_physicsObjects.clear();

		_IsRunning = false;
	}


	void Physics::SearchGameObject(std::shared_ptr<GameObject> gameObject)
	{
		for (auto& child : gameObject->children)
		{
			SearchGameObject(child);
		}

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
