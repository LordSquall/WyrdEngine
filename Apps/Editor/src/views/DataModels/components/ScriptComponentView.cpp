#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/MonoUtils.h>

/* local includes */
#include "ScriptComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"
#include "views/DataModels/components/ScriptProperties/UnknownPropertyView.h"

/* external includes */
#include <imgui.h>
#include <core/Log.h>

namespace Osiris::Editor
{
	ScriptComponentView::ScriptComponentView(ScriptComponent* baseComponent) : _BaseComponent(baseComponent)
	{
		if (_BaseComponent->Properties != nullptr)
		{
			for (auto& prop : *_BaseComponent->Properties)
			{
				std::unique_ptr<ScriptPropertyView> view = std::move(ScriptPropertyViewFactory::Create(prop));
				if (view != nullptr)
				{
					_PropertyViews.push_back(std::move(view));
				}
				else
				{
					_PropertyViews.push_back(std::make_unique<UnknownPropertyView>(prop));
				}
			}
		}
	}

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

		for (auto& prop : _PropertyViews)
		{
			ImGui::PushID(&prop);
			prop->DrawUI();
			ImGui::PopID();
		}

	}

	

	/*
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
	}*/
}