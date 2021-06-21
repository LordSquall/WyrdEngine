
/* core wyrd includes */
#include <core/export.h>
#include <events/KeyEvent.h>

/* local includes */
#include "views/EditorViewBase.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class FileViewer : public EditorViewBase
	{
	public:
		FileViewer(EditorLayer* editorLayer);
		~FileViewer() = default;

		void OnEditorRender() override;

	private:
		/* Services*/
		std::shared_ptr<ResourceService> _resourcesService;
		std::shared_ptr<WorkspaceService> _workspaceService;
		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<DialogService> _dialogService;
	};
}