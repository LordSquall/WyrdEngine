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
		PhysicsComponent(GameObject* gameObject);
		PhysicsComponent(const PhysicsComponent& obj);
		~PhysicsComponent();

		void Recalculate() override;

		inline virtual const std::string GetManagedType() { return "PhysicsComponent"; }

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

		void AddCollisionState(PhysicsComponent* collisionKey);
		unsigned int CurrentCollisionState(PhysicsComponent* collisionKey);
		void RemoveCollisionState(PhysicsComponent* collisionKey);

		/**
		 * @brief See IBaseComponent::ToJson()
		*/
		virtual jsonxx::Object ToJson() override;

		/**
		 * @brief See IBaseComponent::FromJson()
		*/
		virtual bool FromJson(jsonxx::Object& object) override;

	public:
		glm::vec2 previousPosition;

	private:
		bool _UseSpriteBoundary;
		bool _IsStatic;
		bool _IsTrigger;
		
		Rect _AABB;

		glm::vec2 _velocity;

		std::map<PhysicsComponent*, unsigned int> _CollisionStates;
	};
}