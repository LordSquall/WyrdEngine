#include "core/export.h"
#include "core/LayerStack.h"

#include "services/ServiceManager.h"

#include "views/EditorViewBase.h"

namespace Osiris::Editor
{
	class PreferencesViewer : public EditorViewBase
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