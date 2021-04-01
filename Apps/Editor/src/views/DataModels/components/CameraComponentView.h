#pragma once

/* core osiris include */
#include <core/scene/components/CameraComponent.h>

/* local includes */
#include "views/PropertiesViewer/IPropertyView.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Osiris;
using namespace glm;

namespace Osiris::Editor {

	class CameraComponentView : public IPropertiesView
	{
	public:
		CameraComponentView(CameraComponent* baseComponent) : _BaseComponent(baseComponent) {}
		CameraComponentView(const CameraComponentView& obj) {}
	
		void Initialise() override;

		void OnPropertyEditorDraw() override;

		const std::string GetName() override { return "Camera"; }

	private:
		void OnSceneViewerRender(Renderer& renderer, Timestep timestep, const glm::mat4& matrix);

	private:
		CameraComponent* _BaseComponent;
	};
}