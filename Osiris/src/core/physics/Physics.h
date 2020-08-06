#pragma once

#include "osrpch.h"
#include "core/export.h"
#include "events/Event.h"
#include "core/Timestep.h"
#include "core/physics/Polygon.h"

#include <glm/glm.hpp>


namespace Osiris
{
	struct PhysicsStaticConfig
	{
		float positionX;
		float positionY;

		float sizeX;
		float sizeY;

		Polygon* polygon;

		bool isTrigger;
	};

	struct PhysicsDynamicConfig
	{
		float* bindingPosX;
		float* bindingPosY;

		float sizeX;
		float sizeY;

		void* data;

		Polygon* polygon;

		bool isTrigger;

		std::function<void(void*)> updateFunc;
	};

	struct PhysicsStaticObject
	{
		uint32_t id;

		float positionX;
		float positionY;

		float sizeX;
		float sizeY;

		Polygon* polygon;

		bool isTrigger;
	};

	struct PhysicsDynamicObject
	{
		uint32_t id;

		float* bindingPosX;
		float* bindingPosY;

		float sizeX;
		float sizeY;

		void* data;
		
		Polygon* polygon;

		bool isTrigger;

		std::function<void(void*)> updateFunc;

		float previousX;
		float previousY;
		
		float positionUpdateX;
		float positionUpdateY;

		std::list<uint32_t> triggeredObjects;
	};

	class OSR_LIBRARY_API Physics
	{
	public:
		Physics();
		virtual ~Physics();

		inline const bool IsRunning() const { return _IsRunning; }
		inline void IsRunning(const bool isRunning) { _IsRunning = isRunning; }

		void Update(Timestep ts);

		void AddStaticObject(PhysicsStaticConfig& config);
		void AddDynamicObject(PhysicsDynamicConfig& config);

		void Reset();

		void OutputAllObjectsToConsole();

	private:
		bool _IsRunning;
		std::vector<PhysicsStaticObject>	_StaticObjects;
		std::vector<PhysicsDynamicObject>	_DynamicObjects;

		static uint32_t s_NextObjectID;
	};
}