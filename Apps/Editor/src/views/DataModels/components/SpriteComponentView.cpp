#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/scene/GameObject.h>

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

		if (ImGui::InputFloat2("Pos ", position, 1) == true)
		{
			_BaseComponent->position.x = position[0];
			_BaseComponent->position.y = position[1];
			_BaseComponent->Owner->inputArea.x = (float)position[0];
			_BaseComponent->Owner->inputArea.y = (float)position[1];
		}

		if (ImGui::InputFloat2("Size", size, 1))
		{
			_BaseComponent->size.x = size[0];
			_BaseComponent->size.y = size[1];
			_BaseComponent->Owner->inputArea.z = (float)size[0];
			_BaseComponent->Owner->inputArea.w = (float)size[1];
		}

		ImGui::Text("Texture");
		ImGui::PushID("texture");
		ImGui::SameLine();
		ImGui::Image((ImTextureID)(INT_PTR)_BaseComponent->texture->GetHandle(), ImVec2(64.0f, 64.0f));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ASSET_PAYLOAD"))
			{
				std::shared_ptr<TextureRes> texture = *(std::shared_ptr<TextureRes>*)payload->Data;
				_BaseComponent->texture = texture->GetTexture();
				_BaseComponent->SetUUID(texture->GetResourceID());
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