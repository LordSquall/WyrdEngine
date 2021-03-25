#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Osiris::Editor
{
	class NewSceneDialog : public EditorViewDialogBase
	{
	public:
		NewSceneDialog(EditorLayer* editorLayer, const std::string& folder);
		virtual ~NewSceneDialog() = default;

		virtual void OnDialogRender();

	private:
		std::string _Folder;
	};
}