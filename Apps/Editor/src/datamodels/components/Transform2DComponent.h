#pragma once

#include <Osiris.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "tools/PropertiesViewer/IPropertyComponent.h"

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class Transform2DComponent : public IPropertiesComponent
	{
	public:
		Transform2DComponent();
		Transform2DComponent(const Transform2DComponent& obj);
		~Transform2DComponent();

		inline void SetPosition(const vec2& pos);
		inline void SetRotation(const float rot);
		inline void SetScale(const vec2& scl);

		void Translate(const vec2& pos);
		void Rotate(const float rot);
		void Scale(const vec2& scl);

		vec2 position;
		float rotation;
		vec2 scale;

		glm::mat4 matrix;

		// Inherited via TransformComponent
		void OnPropertyEditorDraw() override;

	private:
		inline void UpdateModelMatrix();
	};
}