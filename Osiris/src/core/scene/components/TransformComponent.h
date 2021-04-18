#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"

/* external includes */

namespace Osiris {

	class OSR_LIBRARY_API TransformComponent : public IBaseComponent
	{
	public:
		TransformComponent(GameObject* gameObject, SceneComponentType componentType);
		TransformComponent(const TransformComponent& obj);

		inline virtual const std::string GetManagedType() { return "Transform2DComponent"; }

		/**
		 * @brief Get the computed model matrix for this game object
		 * @return model matrix
		*/
		virtual const glm::mat4& GetModelMatrix() = 0;

		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		virtual bool ToJson(jsonxx::Object& object) = 0;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;
	};
}