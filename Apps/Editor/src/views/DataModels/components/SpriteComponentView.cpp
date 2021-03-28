#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/scene/GameObject.h>
#include <core/Application.h>
#include <core/Resources.h>

/* local includes */
#include "SpriteComponentView.h"
#include "services/ServiceManager.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	void SpriteComponentView::OnPropertyEditorDraw()
	{
		float position[2];
		float size[2];

		float color[3];

		position[0] = _BaseComponent->position.x;
		position[1] = _BaseComponent->position.y;
		size[0] = _BaseComponent->size.x;
		size[1] = _BaseComponent->size.y;
		color[0] = _BaseComponent->color.r;
		color[1] = _BaseComponent->color.g;
		color[2] = _BaseComponent->color.b;

		if (ImGui::InputFloat2("Pos ", position) == true)
		{
			_BaseComponent->SetPosition(position[0], position[1]);
		}

		if (ImGui::InputFloat2("Size", size))
		{
			_BaseComponent->SetSize(size[0], size[1]);
		}

		ImGui::Text("Texture");
		ImGui::PushID("texture");
		ImGui::SameLine();

		auto texture = _BaseComponent->GetTexture();

		if (texture == nullptr) texture = Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)];

		ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				UID* textureUID = (UID*)payload->Data;
				_BaseComponent->SetTexture(Application::Get().GetResources().Textures[*textureUID]);
				_BaseComponent->SetSize((float)texture->GetWidth(), (float)texture->GetHeight());
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();

		if (ImGui::ColorEdit3("Color", color) == true)
		{
			_BaseComponent->color.r = color[0];
			_BaseComponent->color.g = color[1];
			_BaseComponent->color.b = color[2];
		}
	}
}