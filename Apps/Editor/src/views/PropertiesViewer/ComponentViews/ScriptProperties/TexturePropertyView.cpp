#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>

/* local includes */
#include "TexturePropertyView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void TexturePropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		char* nullCheck = (char*)value;
		UID textureUID = UID((char*)value);

		if (*nullCheck == '\0')
		{
			ImGui::Text("TexturePropertyView: Not Set");
		}
		else
		{
			ImGui::Text("TexturePropertyView: %s", textureUID.str().c_str());

			Texture* texture = &*(Application::Get().GetResources().Textures[textureUID]);

			if (texture == nullptr)
			{
				texture = &*(Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)]);
			}

			ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				UID* textureUID = (UID*)payload->Data;
				strcpy((char*)value, textureUID->str().c_str());
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(TexturePropertyView);
}