#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"
#include "datamodels/CameraController.h"
#include "gizmos/Gizmo.h"

#include <core/renderer/Buffer.h>
#include <core/renderer/Shader.h>

namespace Wyrd::Editor
{
	class Grid3DGizmo;
	class Translation2DGizmo;

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

		inline const Wyrd::Rect& GetViewport() { return _Viewport; }
		inline const std::shared_ptr<CameraController> GetCamera() const { return _CameraController; }

		inline const glm::vec2 GetMouseWorldPos() const { return _MouseWorldPos; }

		glm::vec2 Convert2DToWorldSpace(const glm::vec2& point);
		glm::vec2 GetViewportSpaceFromPoint(const glm::vec2& point);
		glm::vec3 GetWorldSpaceFromPoint(const glm::vec3& point); 
		glm::vec2 GetScreenSpaceFromWorldPoint(const glm::vec2& point);


		inline std::shared_ptr<Scene> GetScene() const { return _Scene; }

		enum class TransformTool { Translate, Rotate, Scale } CurrentTransformTool;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e, void* data);
		bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);
		bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e, void* data);

		void OnSceneOpened(Events::EventArgs& args);
		void OnSelectedEntityChanged(Events::EventArgs& args);

	private:
		std::shared_ptr<WorkspaceService>		_Workspace;
		std::shared_ptr<EventService>			_Events;
		std::shared_ptr<ResourceService>		_Resources;
		std::shared_ptr<SettingsService>		_Settings;
		std::shared_ptr<DialogService>			_Dialogs;
		std::shared_ptr<SimulationService>		_Simulation;
		std::shared_ptr<CoreSystemsService>		_CoreSystem;
		std::unique_ptr<Wyrd::FrameBuffer>		_Framebuffer;

		std::shared_ptr<Scene>					_Scene;
		std::shared_ptr<CameraController>		_CameraController;
		std::shared_ptr<Shader>					_Shader;


		std::unique_ptr<Grid3DGizmo>			_Grid3DGizmo;
		std::shared_ptr<Shader>					_GizmoShader;

		Entity	_SelectedEntity;

		glm::vec2 _LastMousePos;
		glm::vec2 _MenuPos;

		glm::vec2 _MouseWorldPos;
		glm::vec2 _PrevMouseWorldPos;
		
		bool _OpenContextMenu;

		bool _mouseEventStarted;

		glm::vec2 _mouseOffset;
		Wyrd::Rect _ViewportBoundary;
		Wyrd::Rect _Viewport;
	};
}