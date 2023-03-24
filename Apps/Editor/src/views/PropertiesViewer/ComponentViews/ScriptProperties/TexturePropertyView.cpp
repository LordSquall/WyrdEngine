/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>
#include <core/behaviour/Properties/TextureProperty.h>

/* local includes */
#include "TexturePropertyView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void TexturePropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		// cast the property
		TextureProperty* textureProperty = dynamic_cast<TextureProperty*>(prop.get());

		UID currentUID = textureProperty->GetValue();

		if (!currentUID.isValid())
		{
			ImGui::Text("TexturePropertyView: Not Set");
		}
		else
		{
			// check we have a value texture in resources
			Texture* texture = Resources::Get().Textures[currentUID].get();

			ImGui::Text("TexturePropertyView: %s", currentUID.str().c_str());
			ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));

		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
			{
				textureProperty->Set(payload->Data);
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(TexturePropertyView);
}