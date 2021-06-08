#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "SpriteComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(SpriteComponentView, "Sprite");

	void SpriteComponentView::OnEditorRender(void* data)
	{
		/* Cast to the correct component */
		SpriteComponent* sprite = (SpriteComponent*)data;

		ImGui::InputFloat2("Pos ", (float*)&sprite->position);
		ImGui::InputFloat2("Size", (float*)&sprite->size);

		//TODO - need to move to util function
		ImGui::Text("Texture");
		ImGui::PushID("texture");
		ImGui::SameLine();

		auto texture = Application::Get().GetResources().Textures[sprite->texture];

		if (texture == nullptr)
			texture = Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)];

		ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				UID* textureUID = (UID*)payload->Data;
				sprite->texture = *textureUID;
				sprite->size = { (float)texture->GetWidth(), (float)texture->GetHeight() };
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();

		ImGui::ColorEdit3("Color", (float*)&sprite->color);
	}
}