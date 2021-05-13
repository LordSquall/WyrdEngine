#pragma once

/* local includes */
#include "core/scene/components/TransformComponent.h"

/* external includes */
#include <glm/glm.hpp>

using namespace glm;

namespace Wyrd {
	
	class GameObject;

	class WYRD_LIBRARY_API Transform3DComponent : public TransformComponent
	{
	public:
		Transform3DComponent(GameObject* gameObject);
		~Transform3DComponent();

		inline virtual const std::string GetManagedType() { return "Transform3DComponent"; }

		/**
		 * @brief See TransformComponent::GetModelMatrix()
		*/
		virtual const glm::mat4& GetModelMatrix() override { return _ModelMatrix; };

		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		virtual bool ToJson(jsonxx::Object& object) override;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/

		virtual bool FromJson(jsonxx::Object& object) override;

		vec3 position;
		vec3 rotation;
		vec3 scale;

	private:
		glm::mat4 _ModelMatrix;
	};
}