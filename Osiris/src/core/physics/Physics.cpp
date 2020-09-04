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

/* external includes */
#include <MathHelper.h>

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
							bool result = Polygon::CollisionTest_DIAG(physicsA->GetPolygon(), physicsB->GetPolygon(), NULL, 0);
							if (result)
							{
								for (auto component : physicsA->Owner->components)
								{
									if (component->GetType() == SceneComponentType::ScriptComponent)
									{
										/* build property arguments */
										std::shared_ptr<ScriptedGameObject> managedGameObject = Application::Get().GetBehaviour().GetGameObject(physicsB->Owner->GetUID());

										std::vector<void*> args = std::vector<void*>({ managedGameObject->Object });
										MonoUtils::ExecuteScriptMethod((ScriptComponent*) & *component, std::string("OnTriggerCollision"), args);
									}
								}
							}
						}
						/* non-trigger on trigger */
						else if (physicsA->IsTrigger() == false && physicsB->IsTrigger() == true)
						{
							bool result = Polygon::CollisionTest_DIAG(physicsA->GetPolygon(), physicsB->GetPolygon(), NULL, 0);
							if (result)
							{
								for (auto component : physicsA->Owner->components)
								{
									if (component->GetType() == SceneComponentType::ScriptComponent)
									{
										/* build property arguments */
										std::shared_ptr<ScriptedGameObject> managedGameObject = Application::Get().GetBehaviour().GetGameObject(physicsB->Owner->GetUID());

										std::vector<void*> args = std::vector<void*>({ managedGameObject->Object });
										MonoUtils::ExecuteScriptMethod((ScriptComponent*) & *component, std::string("OnTriggerCollision"), args);
									}
								}
							}
						}
						/* non-trigger on non-trigger */
						else if (physicsA->IsTrigger() == false && physicsB->IsTrigger() == false)
						{
							if (Rect::Contains(physicsA->GetAABB(), physicsB->GetAABB()) == true)
							{
								// perform a north check
								if (physicsB->IsStatic() == true)
								{
									vec2 collisionDepth = Rect::GetDepth(physicsA->GetAABB(), physicsB->GetAABB());

									physicsA->Owner->transform2D->Translate({ collisionDepth.x, 0.0f });

									physicsA->Update(0.0f);
								}

								//if (physicsB->IsStatic() == true)
								//{
									//player.x += velocity.x;
									//player.updateAABB();
									//for (int tx = left - 1; tx <= right + 1; tx++)
									//{
									//	for (int ty = top - 1; ty <= bottom + 1; ty++)
									//	{
									//		aabb = world.getTileAABB(tx, ty);
									//		if (aabb.collidesWith(player.aabb))
									//		{
									//			depth = player.aabb.getXDepth(aabb);
									//			player.x -= depth;
									//		}
									//	}
									//}
									
									//player.y += velocity.y;
									//player.updateAABB();
									//for (int tx = left - 1; tx <= right + 1; tx++)
									//{
									//	for (int ty = top - 1; ty <= bottom + 1; ty++)
									//	{
									//		aabb = world.getTileAABB(tx, ty);
									//		if (aabb.collidesWith(player.aabb))
									//		{
									//			depth = player.aabb.getYDepth(aabb);
									//			player.y -= depth;
									//		}
									//	}
									//}
								//}
							}
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
