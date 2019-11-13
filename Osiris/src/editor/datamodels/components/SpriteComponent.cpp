#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "SpriteComponent.h"

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
		int x, y, w, h;

		x = Sprite->GetX();
		y = Sprite->GetY();
		w = Sprite->GetWidth();
		h = Sprite->GetHeight();

		if (ImGui::TreeNode("Sprite Renderer"))
		{
			ImGui::Text("Position");
			ImGui::Text("x");
			ImGui::PushID("pos.x");
			ImGui::SameLine();
			if (ImGui::InputInt("##hidelabel", &x) == true)
				Sprite->SetX(x);
			ImGui::PopID();

			ImGui::Text("y");
			ImGui::PushID("pos.y");
			ImGui::SameLine();
			if(ImGui::InputInt("##hidelabel", &y) == true)
				Sprite->SetY(y);
			ImGui::PopID();

			ImGui::Text("Texture");
			ImGui::PushID("texture");
			ImGui::SameLine();
			ImGui::Image((ImTextureID)BaseTexture->GetRendererHandle(), ImVec2(100.0f, 100.0f));
			ImGui::PopID();

			ImGui::TreePop();
		}
	}
}