/* core wyrd includes */
#include <core/export.h>

/* local includes */
#include "ComponentViewFactory.h"
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyView.h"
#include "views/PropertiesViewer/ComponentViews/ScriptProperties/ScriptPropertyViewFactory.h"

namespace Wyrd::Editor
{
	class ScriptComponentView
	{
	public:
		COMPONENT_VIEW_FACTORY_SETUP(ScriptComponentView);

	private:
		void RebuildPropertyViews();

	private:
		std::vector<std::unique_ptr<ScriptPropertyView>> _PropertyViews;
	};
}