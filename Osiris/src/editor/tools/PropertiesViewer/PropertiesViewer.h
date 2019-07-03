#include "core/export.h"
#include "editor/EditorPlugin.h"

namespace Osiris::Editor
{
	class PropertiesViewer : public EditorPlugin
	{
	public:
		PropertiesViewer();
		~PropertiesViewer();

		void OnEditorRender() override;
	};
}