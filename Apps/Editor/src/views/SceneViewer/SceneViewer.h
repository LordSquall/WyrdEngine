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
	class GridGizmo;

	class SceneViewer : public EditorViewBase
	{
	public:
		SceneViewer(EditorLayer* editorLayer);
		~SceneViewer();

		/* EditorPlugin functions */
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
		void OnEditorRender() override;

		void OnResize() override;

		inline const Osiris::Rect& GetViewport() { return _Viewport; }
		inline const std::shared_ptr<OrthographicCameraController> GetCamera() const { return _CameraController; }

		inline const glm::vec2 GetMouseWorldPos() const { return _MouseWorldPos; }

		glm::vec2 Convert2DToWorldSpace(const glm::vec2& point);
		glm::vec2 GetViewportSpaceFromPoint(const glm::vec2& point);
		glm::vec2 GetWorldSpaceFromPoint(const glm::vec2& point); 
		glm::vec2 GetScreenSpaceFromWorldPoint(const glm::vec2& point);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);

		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedGameObjectChanged(Events::EventArgs& args);

		void RenderGameObject(std::shared_ptr<GameObject> gameObject, Timestep ts, Renderer& renderer);
		void UpdateGameObject(std::shared_ptr<GameObject> gameObject, Timestep ts, bool updateTransform);

		std::shared_ptr<GameObject> FindGameObjectInScene(glm::vec2 inputPosition);
		std::shared_ptr<GameObject> FindGameObjectInGameObject(std::shared_ptr<GameObject>, glm::vec2 inputPosition);

	private:
		std::shared_ptr<WorkspaceService>		_WorkspaceService;
		std::shared_ptr<EventService>			_EventService;
		std::shared_ptr<ResourceService>		_ResourceService;
		std::shared_ptr<SettingsService>		_SettingsService;
		std::shared_ptr<DialogService>			_DialogService;
		std::shared_ptr<SimulationService>		_SimulationService;
		std::unique_ptr<Osiris::FrameBuffer>	_Framebuffer;

		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<OrthographicCameraController>	_CameraController;
		std::shared_ptr<Shader>							_Shader;


		std::unique_ptr<GridGizmo>						_GridGizmo;
		std::unique_ptr<TranslationGizmo>				_TranslationGizmo;
		std::shared_ptr<Shader>							_GizmoShader;
		std::shared_ptr<GameObject>						_SelectedGameObject;

		glm::vec2 _LastMousePos;
		glm::vec2 _MenuPos;

		glm::vec2 _MouseWorldPos;
		glm::vec2 _PrevMouseWorldPos;
		
		bool _OpenContextMenu;

		bool _mouseEventStarted;

		glm::vec2 _mouseOffset;
		Osiris::Rect _ViewportBoundary;
		Osiris::Rect _Viewport;
	};
}