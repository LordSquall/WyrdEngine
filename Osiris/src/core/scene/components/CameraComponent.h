#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"
#include "core/Structures.h"
#include "core/pipeline/OrthographicCamera.h"

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

		inline void SetSize(float size) { _Size = size; }
		inline float GetSize() const { return _Size; }

		inline void SetCameraUID(const UID& uid) { _CameraUID = uid; }
		inline const UID& GetCameraUID() const { return _CameraUID; }

		inline OrthographicCamera& GetCamera() const { return *_Camera; }
						
		/**
		 * @brief Serialise the gameobject into a json object
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool ToJson(jsonxx::Object& object) override;

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		virtual bool FromJson(jsonxx::Object& object) override;

		std::string toString() const { return "CameraComponent"; }

	private:
		float _Size;

		UID _CameraUID;
		std::unique_ptr<OrthographicCamera> _Camera;
	};
}