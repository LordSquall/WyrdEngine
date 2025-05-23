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

	void SpriteComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		SpriteComponent* sprite = (SpriteComponent*)data;
		
		ImGui::InputFloat2("Pos ", (float*)&sprite->position);
		ImGui::InputFloat2("Size", (float*)&sprite->size);
		ImGui::InputFloat2("Tiling", (float*)&sprite->tiling);
		
		//TODO - need to move to util function
		ImGui::Text("Texture");
		//ImGui::PushID("texture");
		//ImGui::SameLine();
		//
		//auto texture = Application::Get().GetResources().Textures[sprite->sprite];
		//
		//if (texture == nullptr)
		//	texture = Application::Get().GetResources().Textures[UID(RES_TEXTURE_DEFAULT)];
		//
		//ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
		//if (ImGui::BeginDragDropTarget())
		//{
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_TEXTURE))
		//	{
		//		UID* spriteUID = (UID*)payload->Data;
		//
		//		/* retrieve the new texture */
		//		auto newTexture = Application::Get().GetResources().Textures[*spriteUID];
		//
		//		if (newTexture)
		//		{
		//			sprite->sprite = *spriteUID;
		//			sprite->size = { (float)newTexture->GetWidth(), (float)newTexture->GetHeight() };
		//		}
		//		else
		//		{
		//			WYRD_ERROR("Unable to find source Texture from Drag and Drop Payload!");
		//		}
		//	}
		//	ImGui::EndDragDropTarget();
		//}
		//ImGui::PopID();
		
		ImGui::ColorEdit3("Color", (float*)&sprite->color);
	}
}