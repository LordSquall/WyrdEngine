/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "BoolPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void BoolPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::Checkbox(prop->GetName().c_str(), (bool*)value);
	}

	bool BoolPropertyView::s_registered = ScriptPropertyViewFactory::Register(
		BoolPropertyView::GetFactoryName(),
		BoolPropertyView::OnEditorRender);
}