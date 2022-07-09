#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

#include "datamodels/RecentProjects.h"

namespace Wyrd::Editor
{
	class NewProjectDialog : public EditorViewDialogBase
	{
	public:
		NewProjectDialog(EditorLayer* editorLayer);
		virtual ~NewProjectDialog() = default;

		virtual void OnDialogRender();

	private:
		RecentProjects _RecentProjects;
	};
}