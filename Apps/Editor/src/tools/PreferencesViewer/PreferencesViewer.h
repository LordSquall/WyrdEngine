#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "EditorPlugin.h"

namespace Osiris::Editor
{
	class PreferencesViewer : public EditorPlugin
	{
	public:
		PreferencesViewer();
		~PreferencesViewer();

		void OnEditorRender() override;

	private:
		void OnEditorRender_RenderDoc();

		std::shared_ptr<SettingsService> _SettingsService;
	};
}