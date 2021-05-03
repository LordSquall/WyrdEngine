#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	class NewScriptDialog : public EditorViewDialogBase
	{
	public:
		NewScriptDialog(EditorLayer* editorLayer, const std::string& folder);
		virtual ~NewScriptDialog() = default;

		virtual void OnDialogRender();

	private:
		std::string _Folder;
	};
}