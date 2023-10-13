#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"

#include <core/renderer/Buffer.h>
#include <core/renderer/Shader.h>

//struct Wyrd::CameraComponent;

namespace Wyrd::Editor
{
	class GameViewer : public EditorViewBase
	{
	public:
		GameViewer(EditorLayer* editorLayer);
		~GameViewer();

		/* EditorPlugin functions */
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
		void OnEditorRender() override;

		void OnResize() override;

		inline const Wyrd::Rect& GetViewport() { return _Viewport; }

	private:
		void OnSceneOpened(Events::EventArgs& args);

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e, void* data);
		bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);
		bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e, void* data);

	private:
		std::shared_ptr<WorkspaceService>		_Workspace;
		std::shared_ptr<EventService>			_Events;
		std::shared_ptr<ResourceService>		_Resources;
		std::shared_ptr<SettingsService>		_Settings;
		std::shared_ptr<SimulationService>		_Simulation;
		std::unique_ptr<Wyrd::FrameBuffer>		_Framebuffer;

		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<Shader>							_Shader;

		Entity _CameraEntity;
		std::shared_ptr<Camera> _Camera;

		bool _OpenContextMenu;

		bool _mouseEventStarted;

		glm::vec2 _mouseOffset;
		Wyrd::Rect _ViewportBoundary;
		Wyrd::Rect _Viewport;

		int _SizeConfigID;
	};
}