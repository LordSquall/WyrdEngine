#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class SystemsDebugView : public EditorViewBase
	{
	public:
		SystemsDebugView(EditorLayer* editorLayer);
		~SystemsDebugView();

		/* EditorPlugin functions */
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
		void OnEditorRender() override;

		void OnResize() override;

	private:
		void OnSceneOpened(Events::EventArgs& args);

		std::shared_ptr<EventService>   _EventService;
		std::shared_ptr<Scene>			_Scene;
	};
}