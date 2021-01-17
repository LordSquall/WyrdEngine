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
		if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen))
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
				default:
					ImGui::Text("%s - Unknown Type", prop.name.c_str());
				}
			}

			ImGui::TreePop();
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
				uint32_t gameObjectID = *(const uint32_t*)payload->Data;
				std::shared_ptr<GameObject> gameObject = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace)->GetLoadedScene()->FindGameObject(gameObjectID);
				
				std::shared_ptr<IBaseComponent> scriptComponent = nullptr;
				std::string className;
				for (auto& c : gameObject->components)
				{
					if(c->GetType() == SceneComponentType::ScriptComponent)
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
					prop.objectVal = gameObjectID;
					prop.objectNameVal = gameObject->name;
					prop.objectClassNameVal = className;
					OSR_TRACE("GameObject found {0} {1}", gameObject->GetUID(), gameObject->name);
				}
				else
				{
					OSR_TRACE("Unable to find matching script component type");
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
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
			{
				uint32_t gameObjectID = *(const uint32_t*)payload->Data;
				std::shared_ptr<GameObject> gameObject = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace)->GetLoadedScene()->FindGameObject(gameObjectID);

				std::shared_ptr<IBaseComponent> scriptComponent = nullptr;
				std::string className = nullptr;
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
					prop.objectVal = gameObjectID;
					prop.objectNameVal = gameObject->name;
					prop.objectClassNameVal = className;
					OSR_TRACE("GameObject found {0} {1}", gameObject->GetUID(), gameObject->name);
				}
				else
				{
					OSR_TRACE("Unable to find matching script component type");
				}

			}
			ImGui::EndDragDropTarget();
		}
	}
}