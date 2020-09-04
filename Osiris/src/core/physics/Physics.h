#pragma once

#include "osrpch.h"
#include "core/export.h"
#include "events/Event.h"
#include "core/Timestep.h"

#include <glm/glm.hpp>

namespace Osiris
{
	class Scene;
	class GameObject;
	class PhysicsComponent;

	class OSR_LIBRARY_API Physics
	{
	public:
		Physics();
		~Physics();

		inline const bool IsRunning() const { return _IsRunning; }

		void Start(std::shared_ptr<Scene> scene);
		void Update(Timestep ts);
		void Reset();
		void Stop();

	private:
		bool _IsRunning;

		std::vector<PhysicsComponent*> _physicsObjects;
	};
}