#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/renderer/Renderer.h"

/* external includes */
#include <crossguid/guid.hpp>

namespace Osiris {

	class GameObject;

	enum class SceneComponentType
	{
		Transform2D = 1,
		Transform3D,
		SpriteRenderer,
		ScriptComponent,
		PhysicsComponent
	};

	class OSR_LIBRARY_API IBaseComponent
	{
	public:
		IBaseComponent(std::shared_ptr<GameObject> gameObject, SceneComponentType type) : Owner(gameObject), _type(type), Initialised(false) {}
		virtual ~IBaseComponent() {};

		inline const SceneComponentType GetType() const { return _type; }

		inline xg::Guid GetUID() const { return _uid; }
		inline void SetUID(xg::Guid uid) { _uid = uid; }

		virtual void Initialise() {};
		virtual void Setup() {};
		virtual void Recalculate() {};
		virtual void Render(Timestep ts, Renderer& renderer) {};
		virtual void Remove() {};

		virtual const std::string GetManagedType() = 0;

		void* ManagedObject;

	public:
		std::shared_ptr<GameObject> Owner;

		bool Initialised;
		std::function<void(const glm::mat4&)> debugOverlayFunction;

	private:
		SceneComponentType _type;

		xg::Guid _uid;

	};
}