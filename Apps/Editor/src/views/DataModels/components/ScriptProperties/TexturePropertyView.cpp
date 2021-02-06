#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/TexturePropertyView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void TexturePropertyView::DrawUI()
	{
		Texture* texture = _Property->GetValue();

		if (texture == nullptr)
		{
			texture = &*(Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)]);
		}
		ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2(64.0f, 64.0f));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				std::shared_ptr<Texture> texture = *(std::shared_ptr<Texture>*)payload->Data;

				_Property->SetValue(texture.get());
				_Property->SetValueUID(&texture->GetUID());
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(TexturePropertyView);
}