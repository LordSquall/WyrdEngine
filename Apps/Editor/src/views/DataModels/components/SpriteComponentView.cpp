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
		int position[2];
		int size[2];

		float color[3];

		position[0] = _BaseComponent->Sprite->GetX();
		position[1] = _BaseComponent->Sprite->GetY();
		size[0] = _BaseComponent->Sprite->GetWidth();
		size[1] = _BaseComponent->Sprite->GetHeight();
		color[0] = _BaseComponent->Color.r;
		color[1] = _BaseComponent->Color.g;
		color[2] = _BaseComponent->Color.b;

		if (ImGui::TreeNodeEx("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::InputInt2("Pos ", position, 1) == true)
			{
				_BaseComponent->Sprite->SetPosition(position[0], position[1]);
				_BaseComponent->Owner->inputArea.x = (float)position[0];
				_BaseComponent->Owner->inputArea.y = (float)position[1];
			}

			if (ImGui::InputInt2("Size", size, 1))
			{
				_BaseComponent->Sprite->SetSize(size[0], size[1]);
				_BaseComponent->Owner->inputArea.z = (float)size[0];
				_BaseComponent->Owner->inputArea.w = (float)size[1];
			}

			ImGui::Text("Texture");
			ImGui::PushID("texture");
			ImGui::SameLine();
			ImGui::Image((ImTextureID)(INT_PTR)_BaseComponent->BaseTexture->GetHandle(), ImVec2(64.0f, 64.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_PAYLOAD"))
				{
					uint32_t payload_n = *(const uint32_t*)payload->Data;
					_BaseComponent->BaseTexture = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetTextureResourceByID(payload_n)->GetTexture();
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			if (ImGui::ColorEdit3("Color", color) == true)
			{
				_BaseComponent->Color.r = color[0];
				_BaseComponent->Color.g = color[1];
				_BaseComponent->Color.b = color[2];
			}

			ImGui::TreePop();
		}
	}
}