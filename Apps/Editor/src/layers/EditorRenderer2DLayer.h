#pragma once

#include <Osiris.h>

#include "events/EditorEvents.h"
#include "layers/Renderer2DLayer.h"
#include "datamodels/OrthographicCameraController.h"
#include "gizmos/TranslationGizmo.h"

using namespace Osiris;

namespace Osiris::Editor
{
	class EditorRenderer2DLayer : public Layers::Renderer2DLayer
	{
	public:
		EditorRenderer2DLayer(std::string name);

		void OnRender(Timestep ts, Renderer& renderer) override;

		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedGameObjectChanged(Events::EventArgs& args);

	private:
		std::shared_ptr<OrthographicCameraController>	_CameraController;
		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<Shader>							_GizmoShader;
		std::shared_ptr<Texture>						_IconsTexture;
		std::shared_ptr<TranslationGizmo>				_TranslationGizmo;
		std::shared_ptr<GameObject>						_SelectedGameObject;
	};
}