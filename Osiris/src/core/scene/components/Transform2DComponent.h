#pragma once

/* local includes */
#include "core/scene/components/IBaseComponent.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace Osiris {

	class OSR_LIBRARY_API Transform2DComponent : public IBaseComponent
	{
	public:
		Transform2DComponent(std::shared_ptr<GameObject> gameObject);
		Transform2DComponent(const Transform2DComponent& obj);

		inline void SetPosition(const vec2& pos);
		inline void SetRotation(const float rot);
		inline void SetScale(const vec2& scl);

		void Translate(const vec2& pos);
		void Rotate(const float rot);
		void Scale(const vec2& scl);

		inline void SetMatrixValid(const bool isValid) { _IsMatrixValid = isValid; }
		inline const bool IsMatrixValid() const { return _IsMatrixValid; }

		void Recalculate();
		
		vec2 position;
		float rotation;
		vec2 scale;

		vec2 globalPosition;

		glm::mat4 matrix;

		std::string toString() const { return "Hello"; }

	private:
		bool _IsMatrixValid;
	};
}