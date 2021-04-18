#pragma once

/* local includes */
#include "core/scene/components/TransformComponent.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Osiris {

	class OSR_LIBRARY_API Transform2DComponent : public TransformComponent
	{
	public:
		Transform2DComponent(GameObject* gameObject);

		inline virtual const std::string GetManagedType() { return "Transform2DComponent"; }

		void SetPosition(const vec2& pos);
		void SetRotation(const float rot);
		void SetScale(const vec2& scl);

		void Translate(const vec2& pos);
		void Rotate(const float rot);
		void Scale(const vec2& scl);

		inline void SetMatrixValid(const bool isValid) { _IsMatrixValid = isValid; }
		inline const bool IsMatrixValid() const { return _IsMatrixValid; }
		
		void ClearTransformationDelta();

		bool Initialise();
		void Recalculate();
		
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

		vec2 position;
		float rotation;
		vec2 scale;

		vec2 globalPosition;

		glm::mat4 worldMatrix;

		std::string toString() const { return "Hello"; }

	private:
		vec2	_PositionDelta;
		float	_RotationDelta;
		vec2	_ScaleDelta;

		bool	_IsMatrixValid;

		glm::mat4 _ModelMatrix;
	};
}