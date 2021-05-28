#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/ecs/Components.h>

/* local includes */
#include "PropertiesViewer.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	Wyrd::Entity PropertiesViewer::_SelectedEntity = 0;

	PropertiesViewer::PropertiesViewer(EditorLayer* editorLayer) : EditorViewBase("Properties", editorLayer)
	{
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Service::Workspace);

		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(PropertiesViewer::OnSelectedGameObjectChanged));
	}

	PropertiesViewer::~PropertiesViewer() {}

	void PropertiesViewer::OnEditorRender()
	{
		DrawGameObjectUI();
	}

	void PropertiesViewer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedEntity = evtArgs.entity;
	}

	void PropertiesViewer::DrawGameObjectUI()
	{
		if (_SelectedEntity == ENTITY_INVALID)
		{
			return;
		}


		/* Create the basic metadata component view. All entities must have this component */
		MetaDataComponent* metaDataComp = _WorkspaceService->GetLoadedScene()->Get<MetaDataComponent>(_SelectedEntity);
		if (metaDataComp == nullptr)
		{
			WYRD_CORE_ERROR("Entity does not contain MetaDataComponent : {0}", _SelectedEntity);
		}

		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::InputText("##label", metaDataComp->name, 32);
		//ImGui::InputText("##label", &(metaDataComp->name), ImGuiInputTextFlags_CallbackCharFilter | ImGuiInputTextFlags_AutoSelectAll, ImGui::InputTextCallback);
		ImGui::Separator();

		/* Process each of the registered component views to see if the entity has a matching assigned component */
		std::bitset<64> mask = _WorkspaceService->GetLoadedScene()->GetMask(_SelectedEntity);

		for (int i = 0; i < _WorkspaceService->GetLoadedScene()->componentPools.size(); ++i)
		{
			if (mask.test(i))
			{
				// TODO
			}
		}

		ECSTransform2DComponent* transformComp = _WorkspaceService->GetLoadedScene()->Get<ECSTransform2DComponent>(_SelectedEntity);
		if (transformComp != nullptr)
		{
			ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f }, "Transform");
			ImGui::InputFloat2("Pos     ", (float*)&transformComp->position);
		}
		ImGui::Separator();

		ECSSpriteComponent* spriteComp = _WorkspaceService->GetLoadedScene()->Get<ECSSpriteComponent>(_SelectedEntity);
		if (spriteComp != nullptr)
		{
			ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Sprite");
			if (ImGui::Button("REMOVED"))
			{
				_WorkspaceService->GetLoadedScene()->RemoveComponent<ECSSpriteComponent>(_SelectedEntity);
			}
			ImGui::InputFloat2("Pos ", (float*)&spriteComp->position);
			ImGui::InputFloat2("Size", (float*)&spriteComp->size);

			//TODO - need to move to util function
			ImGui::Text("Texture");
			ImGui::PushID("texture");
			ImGui::SameLine();

			auto texture = Application::Get().GetResources().Textures[spriteComp->texture];

			ImGui::Image((ImTextureID)(INT_PTR)texture->GetHandle(), ImVec2((float)texture->GetWidth(), (float)texture->GetHeight()));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
				{
					UID* textureUID = (UID*)payload->Data;
					spriteComp->texture = *textureUID;
					spriteComp->size = { (float)texture->GetWidth(), (float)texture->GetHeight() };
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();

			ImGui::ColorEdit3("Color", (float*)&spriteComp->color);
		}
		ImGui::Separator();

		ECSScriptComponent* scriptComp = _WorkspaceService->GetLoadedScene()->Get<ECSScriptComponent>(_SelectedEntity);
		if (scriptComp != nullptr)
		{
			ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Script");
			ImGui::Text("Script: %s", scriptComp->script.str().c_str());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SCRIPT"))
				{
					UID* scriptUID = (UID*)payload->Data;
					scriptComp->script = *scriptUID;
				}
				ImGui::EndDragDropTarget();
			}
		}
		ImGui::Separator();

		ECSCameraComponent* cameraComp = _WorkspaceService->GetLoadedScene()->Get<ECSCameraComponent>(_SelectedEntity);
		if (cameraComp != nullptr)
		{
			ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Camera");
			ImGui::InputFloat("Size     ", &cameraComp->size);
		}
		ImGui::Separator();
	}
}