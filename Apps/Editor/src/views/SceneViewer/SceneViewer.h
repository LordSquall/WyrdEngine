#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"

#include <core/renderer/Buffer.h>
#include <core/renderer/Shader.h>

namespace Osiris::Editor
{
	class TranslationGizmo;

	class SceneViewer : public EditorViewBase
	{
	public:
		SceneViewer();
		~SceneViewer();

		/* EditorPlugin functions */
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
		void OnEditorRender() override;

		void OnResize() override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);

		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedGameObjectChanged(Events::EventArgs& args);

	private:
		std::shared_ptr<WorkspaceService>		_WorkspaceService;
		std::shared_ptr<EventService>			_EventService;
		std::unique_ptr<Osiris::FrameBuffer>	_Framebuffer;

		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<OrthographicCameraController>	_CameraController;
		std::shared_ptr<Shader>							_Shader;

		std::shared_ptr<TranslationGizmo>				_TranslationGizmo;
		std::shared_ptr<Shader>							_GizmoShader;
		std::shared_ptr<GameObject>						_SelectedGameObject;

		glm::vec2 _LastMousePos;
		glm::vec2 _MenuPos;
		bool _OpenContextMenu;

		bool _mouseEventStarted;

		glm::vec2 _mouseOffset;
		glm::vec4 _Viewport;
	};
}