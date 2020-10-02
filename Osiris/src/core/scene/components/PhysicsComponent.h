#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"
#include "core/maths/Rect.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Osiris {

	class GameObject;

	class OSR_LIBRARY_API PhysicsComponent : public IBaseComponent
	{
	public:
		PhysicsComponent(std::shared_ptr<GameObject> owner);
		PhysicsComponent(const PhysicsComponent& obj);
		~PhysicsComponent();

		inline void SetUseSpriteBoundary(const bool useSpriteBoundary) { _UseSpriteBoundary = useSpriteBoundary; }
		inline const bool UseSpriteBoundary() const { return _UseSpriteBoundary; }

		inline void SetIsStatic(const bool isStatic) { _IsStatic = isStatic; }
		inline const bool IsStatic() const { return _IsStatic; }

		inline void SetIsTrigger(const bool isTrigger) { _IsTrigger = isTrigger; }
		inline const bool IsTrigger() const { return _IsTrigger; }

		inline void SetVelocity(const glm::vec2 velocity) { _velocity = velocity; }
		inline glm::vec2 GetVelocity() const { return _velocity; }

		inline Rect GetAABB() const { return _AABB; }

		void Update(Timestep ts);

	public:
		glm::vec2 previousPosition;

	private:
		bool _UseSpriteBoundary;
		bool _IsStatic;
		bool _IsTrigger;
		
		Rect _AABB;

		glm::vec2 _velocity;
	};
}