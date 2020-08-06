#include "osrpch.h"
#include "core/Log.h"

#include "Physics.h"

#include <MathHelper.h>

namespace Osiris
{
	uint32_t Physics::s_NextObjectID = 0u;

	Physics::Physics()
	{
		/* default running state */
		_IsRunning = false;
	}
	
	Physics::~Physics()
	{

	}

	void Physics::Update(Timestep ts)
	{
		if (_IsRunning == true)
		{
			for (auto& binding : _DynamicObjects)
			{
				/* calculate position external position delta */
				binding.positionUpdateX = (*binding.bindingPosX) - binding.previousX;
				binding.positionUpdateY = (*binding.bindingPosY) - binding.previousY;
				
				/* dynamic objects are under the positional control of the physics system, so revert the position update */
				(*binding.bindingPosX) -= binding.positionUpdateX;
				(*binding.bindingPosY) -= binding.positionUpdateY;
			}

			/* start basic collision with static objects */
			for (auto& bindingA : _DynamicObjects)
			{
				for (auto& bindingB : _DynamicObjects)
				{
					/* we don't want to evaluate self colisions */
					if (bindingA.id != bindingB.id)
					{
						/* TODO - Not yet implemented */
					}
				}

				for (auto& bindingB : _StaticObjects)
				{
					/* we don't want to evaluate self colisions */
					if (bindingA.id != bindingB.id)
					{
						glm::vec2 normal = { 0.0f, 0.0f };

						if (bindingB.isTrigger == true)
						{
							bool result = Polygon::CollisionTest_DIAG(bindingA.polygon, bindingB.polygon, NULL, 0);

							bool previouslyTriggered = std::find(bindingA.triggeredObjects.begin(), bindingA.triggeredObjects.end(), 0) == bindingA.triggeredObjects.end();

							if (result == true)
							{
								if (previouslyTriggered == false)
								{
									bindingA.triggeredObjects.push_back(bindingB.id);
									// fire on triggered enter
								}
							}
							else
							{
								if (previouslyTriggered == true)
								{
									bindingA.triggeredObjects.remove(bindingB.id);
									// fire on trigger exit
								}
							}
						}
						else
						{
							Polygon::CollisionTest_DIAG(bindingA.polygon, bindingB.polygon, &normal, 0);
						}

						bindingA.positionUpdateX += normal.x;
						bindingA.positionUpdateY += normal.y;
					}
				}
			}

			for (auto& binding : _DynamicObjects)
			{
				/* apply transformation update */
				(*binding.bindingPosX) += binding.positionUpdateX;
				(*binding.bindingPosY) += binding.positionUpdateY;

				/* store previous value */
				binding.previousX = (*binding.bindingPosX);
				binding.previousY = (*binding.bindingPosY);

				/* called the update */
				binding.updateFunc(binding.data);
			}
		}
	}

	void Physics::AddStaticObject(PhysicsStaticConfig& config)
	{
		/* create a new static physics object */
		PhysicsStaticObject staticObject;
		staticObject.id = s_NextObjectID;
		staticObject.positionX = config.positionX;
		staticObject.positionY = config.positionY;
		staticObject.sizeX = config.sizeX;
		staticObject.sizeY = config.sizeY;
		staticObject.polygon = config.polygon;
		staticObject.isTrigger = config.isTrigger;

		/* add binding */
		_StaticObjects.push_back(staticObject);

		/* increment the object id */
		s_NextObjectID++;
	}

	void Physics::AddDynamicObject(PhysicsDynamicConfig& config)
	{		
		/* create a new dynamic physics object */
		PhysicsDynamicObject dynamicObject;
		dynamicObject.id = s_NextObjectID;
		dynamicObject.bindingPosX = config.bindingPosX;
		dynamicObject.bindingPosY = config.bindingPosY;
		dynamicObject.sizeX = config.sizeX;
		dynamicObject.sizeY = config.sizeY;
		dynamicObject.data = config.data;
		dynamicObject.polygon = config.polygon;
		dynamicObject.updateFunc = config.updateFunc;
		dynamicObject.previousX = (*config.bindingPosX);
		dynamicObject.previousY = (*config.bindingPosY);
		dynamicObject.isTrigger = config.isTrigger;

		/* add binding */
		_DynamicObjects.push_back(dynamicObject);

		/* increment the object id */
		s_NextObjectID++;
	}

	void Physics::Reset()
	{
		for (auto& dynamicObject : _DynamicObjects)
		{
			
		}

		_DynamicObjects.clear();

		for (auto& staticObject : _StaticObjects)
		{
			
		}

		_StaticObjects.clear();

		s_NextObjectID = 0u;
	}

	void Physics::OutputAllObjectsToConsole()
	{
		OSR_INFO("Physics Subsystem Output:");
		OSR_INFO("\tStatic Obects: {0}", _StaticObjects.size());
		for (auto& staticObjects : _StaticObjects)
		{
			//const b2AABB& aabb = staticBinding->GetAABB(0);
			//OSR_INFO("\t\t{0}, {1}, {2}, {3}", aabb.lowerBound.x, aabb.lowerBound.y, aabb.upperBound.x, aabb.upperBound.y);
		}

		OSR_INFO("\tDynamic Objects: {0}", _DynamicObjects.size());
		for (auto& dynamicObjects : _DynamicObjects)
		{
			//OSR_INFO("\t\t{0}, {1}", dynamicBinding.body->GetPosition().x, dynamicBinding.body->GetPosition().y);
		}

	}
}
