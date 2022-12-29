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
		Texture** indirectTexture = (Texture**)value;

		if (*indirectTexture == nullptr)
		{
			ImGui::Text("TexturePropertyView: Not Set");
		}
		else
		{
			ImGui::Text("TexturePropertyView: %s", (*indirectTexture)->GetUID().str().c_str());

			Texture* texture = (*indirectTexture);
				
			ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				UID* textureUID = (UID*)payload->Data;

				(*((Texture**)value)) = Resources::Get().Textures[*textureUID].get();
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(TexturePropertyView);
}