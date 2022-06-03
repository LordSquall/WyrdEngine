/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "IntPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void IntPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::DragInt(prop->GetName().c_str(), (int*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(IntPropertyView);
}