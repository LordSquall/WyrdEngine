#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "SpriteComponent.h"

#include "editor/services/ServiceManager.h"
#include "editor/services/ResourceService.h"

namespace Osiris::Editor
{
	SpriteComponent::SpriteComponent()
	{

	}

	SpriteComponent::SpriteComponent(const SpriteComponent& obj)
	{
		BaseTexture = obj.BaseTexture;
		Sprite = obj.Sprite;
	}

	SpriteComponent::~SpriteComponent()
	{

	}

	void SpriteComponent::OnPropertyEditorDraw()
	{
		int position[2];
		int size[2];

		position[0] = Sprite->GetX();
		position[1] = Sprite->GetY();
		size[0] = Sprite->GetWidth();
		size[1] = Sprite->GetHeight();

		if (ImGui::TreeNode("Sprite Renderer"))
		{

			if (ImGui::InputInt2("Pos ", position, 1) == true)
			{
				Sprite->SetPosition(position[0], position[1]);
			}

			if (ImGui::InputInt2("Size", size, 1))
			{
				Sprite->SetSize(size[0], size[1]);
			}

			ImGui::Text("Texture");
			ImGui::PushID("texture");
			ImGui::SameLine();
			ImGui::Image((ImTextureID)BaseTexture->GetRendererHandle(), ImVec2(64.0f, 64.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_PAYLOAD"))
				{
					uint32_t payload_n = *(const uint32_t*)payload->Data;
					BaseTexture = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetTextureByUID(payload_n);
					Sprite->SetTexture(BaseTexture->GetTexture());
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			ImGui::TreePop();
		}
	}
}