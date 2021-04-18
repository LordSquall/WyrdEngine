#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/renderer/Renderer.h"
#include "core/scene/GameObject.h"

/* external includes */
#include <crossguid/guid.hpp>

namespace Osiris {

	class GameObject;

	enum class SceneComponentType
	{
		Transform2D = 1,
		Transform3D = 2,
		SpriteRenderer = 3,
		ScriptComponent = 4,
		PhysicsComponent = 5,
		CameraComponent = 6
	};

	class OSR_LIBRARY_API IBaseComponent
	{
	public:
		IBaseComponent(GameObject* gameObject, SceneComponentType type) : ManagedObject(nullptr), Owner(gameObject), _type(type), Initialised(false){}
		virtual ~IBaseComponent() {};

		inline const SceneComponentType GetType() const { return _type; }

		inline xg::Guid GetUID() const { return _uid; }
		inline void SetUID(xg::Guid uid) { _uid = uid; }

		virtual bool Initialise() { return true; };
		virtual void Setup() {};
		virtual void Recalculate() {};
		virtual void Render(Timestep ts, Renderer& renderer) {};
		virtual void Remove() {};

		virtual const std::string GetManagedType() = 0;


		/**
		 * @brief Serialise the gameobject into a json object
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool ToJson(jsonxx::Object& object) = 0;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;

		void* ManagedObject;

	public:
		GameObject* Owner;

		bool Initialised;
		std::function<void(Renderer&, Timestep, const glm::mat4&)> debugOverlayFunction;

	private:
		SceneComponentType _type;

		xg::Guid _uid;

	};
}