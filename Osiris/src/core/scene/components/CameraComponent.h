#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Osiris {

	class OSR_LIBRARY_API CameraComponent : public IBaseComponent
	{
	public:
		CameraComponent(GameObject* gameObject);
		~CameraComponent() = default;

		inline virtual const std::string GetManagedType() { return "CameraComponent"; }

		inline void SetPosition(const vec2& pos) { _Position = pos; }
		inline const vec2& GetPosition() const { return _Position; }
				
		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		virtual jsonxx::Object ToJson() override;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) override;

		std::string toString() const { return "CameraComponent"; }

	private:
		vec2 _Position;
	};
}