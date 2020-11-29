#pragma once

/* local includes  */
#include "core/export.h"
#include "core/Timestep.h"
#include "core/renderer/Renderer.h"

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
		IBaseComponent(std::shared_ptr<GameObject> gameObject, SceneComponentType type) : Owner(gameObject), _type(type) {}
		virtual ~IBaseComponent() {};

		inline const SceneComponentType GetType() const { return _type; }


		virtual void Initialise() {};
		virtual void Recalculate() {};
		virtual void Render(Timestep ts, Renderer& renderer) {};

	public:
		std::shared_ptr<GameObject> Owner;

		std::function<void(const glm::mat4&)> debugOverlayFunction;

	private:
		SceneComponentType _type;

	};
}