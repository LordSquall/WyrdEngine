/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>

/* local includes */
#include "EntityPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void EntityPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		char* nullCheck = (char*)value;
		Entity* entity = (Entity*)value;

		if (*nullCheck == '\0')
		{
			ImGui::Text("EntityPropertyView: Not Set");
		}
		else
		{
			ImGui::Text("EntityPropertyView: %llu", *entity);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ENTITY"))
			{
				Entity* entity = (Entity*)payload->Data;
				memcpy(value, entity, sizeof(Entity));
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(EntityPropertyView);
}