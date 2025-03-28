/* core wyrd includes */
#include <core/export.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "events/EditorEvents.h"
#include "datamodels/resources/Resource.h"

namespace Wyrd::Editor
{
	class EventService;
	class WorkspaceService;
	class ResourceService;

	class PropertiesViewer : public EditorViewBase
	{
	public:
		PropertiesViewer(EditorLayer* editorLayer);
		~PropertiesViewer();

		/* EditorPlugin functions */
		void OnEditorRender() override;

	private:
		void OnSelectedEntityChanged(Events::EventArgs& args);
		void OnSelectedResourceChanged(Events::EventArgs& args);

		void DrawEntityUI();
		void DrawResourceUI();

		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
		std::shared_ptr<ResourceService> _ResourceService;

		Wyrd::Entity _SelectedEntity;
		std::shared_ptr<Wyrd::Editor::Resource> _SelectedAsset;

		std::function<void()> _GUIFunc;
	};
}