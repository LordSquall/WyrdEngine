#pragma once

#include "osrpch.h"
#include "core/export.h"
#include "events/Event.h"
#include "core/Timestep.h"

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

		void AddObject(PhysicsComponent* object);
		void RemoveObject(PhysicsComponent* object);

	private:
		void SearchGameObject(std::shared_ptr<GameObject> gameObject);

	private:
		bool _IsRunning;

		std::vector<PhysicsComponent*> _physicsObjects;
	};
}