#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewDialogBase.h"

namespace Wyrd::Editor
{
	class PreferencesDialog : public EditorViewDialogBase
	{
	public:
		PreferencesDialog(EditorLayer* editorLayer);
		~PreferencesDialog();

		virtual void OnDialogRender();

		void OnEditorRender_General();
		void OnEditorRender_RenderDoc();
		void OnEditorRender_SceneViewer();
		void OnEditorRender_WindowsPlayer();
		void OnEditorRender_ExternalTools();

	private:
		std::shared_ptr<SettingsService> _SettingsService;
	};
}