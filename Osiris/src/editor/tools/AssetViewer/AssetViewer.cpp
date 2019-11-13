#pragma once

#include "osrpch.h"

#include "AssetViewer.h"

#include "editor/services/ServiceManager.h"
#include "core/Application.h"
#include "core/Layer.h"

#include "imgui.h"

namespace Osiris::Editor
{
	AssetViewer::AssetViewer() : EditorPlugin("Asset Viewer") 
	{
		/* cache the resource service */
		_resourcesService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
	}

	AssetViewer::~AssetViewer() {}

	void AssetViewer::OnEditorRender()
	{
		float navigationPanelWidth = 100.0f;
		float itemGroupWidth = 64.0f;
		static ResourceService::Type selectedType = ResourceService::Type::NONE;

		ImGui::Begin("Asset Viewer");

		ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y));

		if (ImGui::Button("Textures", ImVec2(navigationPanelWidth, 0)) == true)
		{
			selectedType = ResourceService::Type::TEXTURE;
		}

		if (ImGui::Button("Shaders", ImVec2(navigationPanelWidth, 0)) == true)
		{
			selectedType = ResourceService::Type::SHADER;
		}

		if (ImGui::Button("Models", ImVec2(navigationPanelWidth, 0)) == true)
		{
			selectedType = ResourceService::Type::MODEL;
		}

		ImGui::EndChildFrame();

		ImGui::SameLine();
		ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

		switch (selectedType)
		{
		case ResourceService::Type::TEXTURE:
			TextureItemGui();
			break;
		default:
			break;
		}

		ImGui::EndChildFrame();

		ImGui::End();
	}


	void AssetViewer::PopulateFolderNode(std::string& dir, std::string& name)
	{
		std::vector<std::string> folders = Utils::GetFolderList(dir, false);

		for (auto folder : folders)
		{
			if (ImGui::TreeNodeEx(folder.c_str()))
			{
				PopulateFolderNode(dir + "/" + folder, folder);

				ImGui::TreePop();
			}
		}
	}

	void AssetViewer::TextureItemGui()
	{
		std::map<uint32_t, std::shared_ptr<TextureRes>> resources = _resourcesService->GetTextures();

		int resIdx = 0;

		for (auto res : resources)
		{
			uint32_t id = res.first;

			ImGui::PushItemWidth(100.0f);
		
			ImGui::PushID(resIdx);
			ImGui::BeginGroup();
			ImGui::ImageButton((ImTextureID)res.second->GetRendererHandle(), ImVec2(64, 64));
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ASSET_DND_PAYLOAD", &id, sizeof(uint32_t));
				ImGui::Image((ImTextureID)res.second->GetRendererHandle(), ImVec2(32, 32));
				ImGui::EndDragDropSource();
			}
			ImGui::Text(res.second->GetName().c_str());
			ImGui::EndGroup();
			ImGui::PopID();

			ImGui::SameLine();
			resIdx++;
		}
	}
}