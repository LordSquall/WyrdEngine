#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "ScriptComponent.h"
#include "datamodels/GameObject.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"

#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	ScriptComponent::ScriptComponent()
	{
		CacheResources();
	}

	ScriptComponent::ScriptComponent(std::shared_ptr<GameObject> owner)
	{
		OwnerGameObject = owner;

		CacheResources();
	}

	ScriptComponent::ScriptComponent(const ScriptComponent& obj)
	{
		_ScriptResource = obj._ScriptResource;
		OwnerGameObject = obj.OwnerGameObject;
		
		CacheResources();
	}

	ScriptComponent::~ScriptComponent()
	{

	}


	void ScriptComponent::SetScriptResource(std::shared_ptr<ScriptRes> scriptResource)
	{
		/* store the script resource */
		_ScriptResource = scriptResource;
	}

	void ScriptComponent::CacheResources()
	{
		_RemoveIcon = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetIconLibrary().GetIcon("common", "op_remove");
	}

	void ScriptComponent::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text((_ScriptResource != nullptr) ? _ScriptResource->GetName().c_str() : "<<unbound>>");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_PAYLOAD"))
				{
					uint32_t payload_n = *(const uint32_t*)payload->Data;
					OwnerGameObject->script.SetScriptResource(ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetScriptByUID(payload_n));
				}
				ImGui::EndDragDropTarget();
			} 
			ImGui::SameLine();
			if (ImGui::IconButton(_RemoveIcon, 1, ImVec2(16.0f, 16.0f), _ScriptResource != nullptr) == true)
			{
				_ScriptResource = nullptr;
			}

			ImGui::TreePop();
		}
	}
}