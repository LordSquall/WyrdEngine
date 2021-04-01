#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"

#include <core/renderer/Buffer.h>
#include <core/renderer/Shader.h>

class Osiris::CameraComponent;

namespace Osiris::Editor
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

		inline const Osiris::Rect& GetViewport() { return _Viewport; }

	private:
		void OnSceneOpened(Events::EventArgs& args);

		void RenderGameObject(std::unique_ptr<GameObject>&, Timestep ts, Renderer& renderer);

	private:
		std::shared_ptr<WorkspaceService>		_WorkspaceService;
		std::shared_ptr<EventService>			_EventService;
		std::shared_ptr<ResourceService>		_ResourceService;
		std::shared_ptr<SettingsService>		_SettingsService;
		std::shared_ptr<SimulationService>		_SimulationService;
		std::unique_ptr<Osiris::FrameBuffer>	_Framebuffer;

		std::shared_ptr<Scene>							_Scene;
		std::shared_ptr<Shader>							_Shader;

		CameraComponent* _CameraComponent;
		bool _OpenContextMenu;

		bool _mouseEventStarted;

		glm::vec2 _mouseOffset;
		Osiris::Rect _ViewportBoundary;
		Osiris::Rect _Viewport;
	};
}