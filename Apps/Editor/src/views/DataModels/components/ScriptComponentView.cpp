#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/MonoUtils.h>

/* local includes */
#include "ScriptComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>
#include <core/Log.h>

namespace Osiris::Editor
{
	void ScriptComponentView::OnPropertyEditorDraw()
	{
		if (_BaseComponent->GetClass() != nullptr)
		{
			ImGui::Text(_BaseComponent->GetClass()->GetName().c_str());
		}
		else
		{
			ImGui::Text("<<unbound>>");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_ASSET_PAYLOAD"))
			{
				std::shared_ptr<ScriptRes> scriptRes = *(std::shared_ptr<ScriptRes>*)payload->Data;
				_BaseComponent->SetClass(scriptRes->Script);
				_BaseComponent->SetUUID(scriptRes->GetResourceID());
			}
			ImGui::EndDragDropTarget();
		}

		for (auto& prop : _BaseComponent->Properties)
		{
			switch (prop.type)
			{
			case PropType::INT:
				DrawIntUI(prop);
				break;
			case PropType::FLOAT:
				DrawFloatUI(prop);
				break;
			case PropType::STRING:
				DrawStringUI(prop);
				break;
			case PropType::GAMEOBJECT:
				DrawGameObjectUI(prop);
				break;
			case PropType::COLOR:
				DrawColorUI(prop);
				break;
			case PropType::TEXTURE:
				DrawTextureResourceUI(prop);
				break;
			default:
				ImGui::Text("%s - Unknown Type", prop.name.c_str());
			}
		}

	}

	void ScriptComponentView::DrawIntUI(PropertyDesc& prop)
	{
		int value = prop.intVal;
		if (ImGui::DragInt(prop.name.c_str(), &value))
		{
			prop.intVal = value;
		}
	}

	void ScriptComponentView::DrawFloatUI(PropertyDesc& prop)
	{
		float value = prop.floatVal;
		if (ImGui::DragFloat(prop.name.c_str(), &value))
		{
			prop.floatVal = value;
		}
	}

	void ScriptComponentView::DrawStringUI(PropertyDesc& prop)
	{
		ImGui::InputText(prop.name.c_str(), &prop.stringVal);
	}

	void ScriptComponentView::DrawGameObjectUI(PropertyDesc& prop)
	{
		ImGui::LabelText(prop.name.c_str(), prop.objectNameVal.c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				UID gameObjectID = *(const UID*)payload->Data;
				std::shared_ptr<GameObject> gameObject = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace)->GetLoadedScene()->FindGameObject(gameObjectID);
				
				std::shared_ptr<IBaseComponent> scriptComponent = nullptr;
				std::string className;

				if (prop.objectClassNameVal == "OsirisAPI.GameObject")
				{
					prop.objectUID = gameObjectID;
					prop.objectNameVal = gameObject->name;
					prop.objectClassNameVal = "GameObject";
					OSR_TRACE("GameObject found {0} {1}", gameObject->uid.str(), gameObject->name);
				}
				else
				{
					for (auto& c : gameObject->components)
					{
						if (c->GetType() == SceneComponentType::ScriptComponent)
						{
							std::shared_ptr<ScriptComponent> sc = std::dynamic_pointer_cast<ScriptComponent>(c);

							if (sc->GetClass()->GetName() == prop.objectClassNameVal)
							{
								scriptComponent = sc;
								className = sc->GetClass()->GetName();
								OSR_TRACE("Script Component: Typename: {0}", sc->GetClass()->GetName());
							}
						}
					}

					if (scriptComponent)
					{
						prop.objectUID = gameObjectID;
						prop.objectNameVal = gameObject->name;
						prop.objectClassNameVal = className;
						OSR_TRACE("GameObject found {0} {1}", gameObject->uid.str(), gameObject->name);
					}
					else
					{
						OSR_TRACE("Unable to find matching script component type");
					}
				}

			}
			ImGui::EndDragDropTarget();
		}
	}

	void ScriptComponentView::DrawTextureResourceUI(PropertyDesc& prop)
	{
		ImGui::LabelText(prop.name.c_str(), prop.objectNameVal.c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE_ASSET_PAYLOAD"))
			{
				std::shared_ptr<TextureRes> texture = *(std::shared_ptr<TextureRes>*)payload->Data;

				prop.objectUID = texture->GetResourceID();
				prop.objectNameVal = texture->GetName();
				prop.objectClassNameVal = "Texture";
				OSR_TRACE("Texture found {0}", texture->GetResourceID().str());
			}
			ImGui::EndDragDropTarget();
		}
	}

	void ScriptComponentView::DrawColorUI(PropertyDesc& prop)
	{
		float channels[4] = { prop.colorVal.r, prop.colorVal.g, prop.colorVal.b, prop.colorVal.a };

		if (ImGui::ColorEdit4(prop.name.c_str(), &channels[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview))
		{
			prop.colorVal.r = channels[0];
			prop.colorVal.g = channels[1];
			prop.colorVal.b = channels[2];
			prop.colorVal.a = channels[3];
		}
	}
}