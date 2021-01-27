#pragma once

/* core osiris includes */
#include <Core/Layer.h>

/* local includes */
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"
#include "gizmos/TranslationGizmo.h"

/* external includes */

using namespace Osiris;

namespace Osiris::Editor
{
	class EditorRenderer2DLayer : public Layer
	{
	public:
		EditorRenderer2DLayer(std::string name);

		void OnRender(Timestep ts, Renderer& renderer) override;

		void OnEvent(Event& event) override;

		void OnGUI();

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedGameObjectChanged(Events::EventArgs& args);

	private:
		std::shared_ptr<EventService>					_EventService;
		std::shared_ptr<OrthographicCameraController>	_CameraController;
		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<Shader>							_GizmoShader;
		std::shared_ptr<Texture>						_IconsTexture;
		std::shared_ptr<TranslationGizmo>				_TranslationGizmo;
		std::shared_ptr<GameObject>						_SelectedGameObject;

		glm::vec2 _LastMousePos;
		glm::vec2 _MenuPos;
		bool _OpenContextMenu;
	};
}