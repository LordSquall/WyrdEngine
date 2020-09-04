#pragma once

/* local includes */
#include "Osiris.h"
#include "core/scene/components/IBaseComponent.h"
#include "core/physics/Polygon.h"

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

		inline Osiris::Polygon* GetPolygon() { return &_Polygon; }

		inline const glm::vec4 GetAABB() const { return _AABB; }

		void Update(Timestep ts);

	public:
		glm::vec2 previousPosition;

	private:
		bool _UseSpriteBoundary;
		bool _IsStatic;
		bool _IsTrigger;

		Osiris::Polygon _Polygon;

		glm::vec4 _AABB;
	};
}