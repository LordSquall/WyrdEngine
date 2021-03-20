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
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		virtual jsonxx::Object ToJson() = 0;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) = 0;
	};
}