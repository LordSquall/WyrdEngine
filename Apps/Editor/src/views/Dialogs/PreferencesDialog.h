#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Osiris::Editor
{
	class PreferencesDialog : public EditorViewDialogBase
	{
	public:
		PreferencesDialog(EditorLayer* editorLayer);
		~PreferencesDialog();

		virtual void OnDialogRender();

		void OnEditorRender_General();
		void OnEditorRender_RenderDoc();

	private:
		std::shared_ptr<SettingsService> _SettingsService;
	};
}