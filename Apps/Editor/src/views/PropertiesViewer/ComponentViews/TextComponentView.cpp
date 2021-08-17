#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "TextComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(TextComponentView, "Text");

	void TextComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		TextComponent* textComp = (TextComponent*)data;

		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::InputText("##label", textComp->content, TEXT_COMP_MAX_LENGTH);

		ImGui::ColorEdit3("Color", (float*)&textComp->color);

		std::vector<const char*> fontTypeList;

		for (auto& fontType : Application::Get().GetResources().FontTypes)
		{
			fontTypeList.push_back(fontType.first.c_str());
		}

		if (ImGui::BeginCombo("##combo", textComp->font)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < fontTypeList.size(); n++)
			{
				bool is_selected = (textComp->font == fontTypeList[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(fontTypeList[n], is_selected))
					std::strcpy(textComp->font, fontTypeList[n]);
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}


		ImGui::DragFloat("Size", (float*)&textComp->size, 2.0f, 128.0f);
	}
}