#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Osiris::Editor
{
	class NewProjectDialog : public EditorViewDialogBase
	{
	public:
		NewProjectDialog(EditorLayer* editorLayer);
		virtual ~NewProjectDialog() = default;

		virtual void OnDialogRender();
	};
}