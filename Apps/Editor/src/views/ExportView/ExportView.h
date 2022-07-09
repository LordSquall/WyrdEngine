#include "core/export.h"
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"

//class Wyrd::CameraComponent;

namespace Wyrd::Editor
{
	class ExportView : public EditorViewBase
	{
	public:
		ExportView(EditorLayer* editorLayer);
		~ExportView();

		/* EditorPlugin functions */
		void OnEditorRender() override;

	private:
		std::shared_ptr<WorkspaceService>		_WorkspaceService;
		std::shared_ptr<EventService>			_EventService;
		std::shared_ptr<ResourceService>		_ResourceService;
		std::shared_ptr<SettingsService>		_SettingsService;
		std::shared_ptr<SimulationService>		_SimulationService;

		UID _SelectedSceneUID;
	};
}