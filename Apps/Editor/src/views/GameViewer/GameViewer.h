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
		void OnEditorRender() override;

		void OnResize() override;

		inline const Wyrd::Rect& GetViewport() { return _Viewport; }

	private:
		void OnSceneOpened(Events::EventArgs& args);

	private:
		std::shared_ptr<WorkspaceService>		_WorkspaceService;
		std::shared_ptr<EventService>			_EventService;
		std::shared_ptr<ResourceService>		_ResourceService;
		std::shared_ptr<SettingsService>		_SettingsService;
		std::shared_ptr<SimulationService>		_SimulationService;
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