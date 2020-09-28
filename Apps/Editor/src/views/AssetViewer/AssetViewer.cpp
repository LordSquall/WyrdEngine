#pragma once

#include "osrpch.h"
#include "core/Log.h"

#include "AssetViewer.h"

#include "services/ServiceManager.h"
#include "core/Application.h"
#include "core/Layer.h"

#include "support/ImGuiUtils.h"
#include "imgui.h"

namespace Osiris::Editor
{
	AssetViewer::AssetViewer(EditorLayer* editorLayer) : EditorViewBase("Asset Viewer", editorLayer)
	{
		/* cache the service(s) */
		_resourcesService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);

		/* cache the icon pointers */
		_UnknownIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_unknown");
		_SceneIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_scene");
		_TextureIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_texture");
		_ScriptIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_script");

		/* register from events */
		_EventService->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args)
			{
				_currentDir = Utils::GetAssetFolder();
			});
	}

	AssetViewer::~AssetViewer() {}

	void AssetViewer::OnEditorRender()
	{
		float navigationPanelWidth = 100.0f;
		float itemGroupWidth = 64.0f;
		static ResourceService::Type selectedType = ResourceService::NONE;

		if (_workspaceService->GetCurrentProject() != nullptr)
		{
		
			ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y));

			PopulateFolderNode(Utils::GetAssetFolder());
						
			ImGui::EndChildFrame();

			ImGui::SameLine();
			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			DrawAssetsItems();
		
			ImGui::EndChildFrame();
		}
	}

	void AssetViewer::PopulateFolderNode(const std::string& dir)
	{
		std::vector<std::string> folders = Utils::GetFolderList(dir, false);

		for (auto folder : folders)
		{
			std::vector<std::string> subFolders = Utils::GetFolderList(dir + "/" + folder, false);

			if (ImGui::TreeNodeEx(folder.c_str(), subFolders.size() == 0 ? ImGuiTreeNodeFlags_Bullet : ImGuiTreeNodeFlags_None))
			{
				if (ImGui::IsItemClicked())
				{
					_currentDir = dir + "/" + folder;
				}
				
				PopulateFolderNode(dir + "/" + folder);

				ImGui::TreePop();
			}
		}
	}


	void AssetViewer::DrawAssetsItems()
	{
		uint32_t resIdx = 0;
		uint32_t rowCnt = 0;
		uint32_t colCnt = 4;
		std::vector<std::string> files = Utils::GetFileList(_currentDir, false);

		ImGui::Columns(colCnt, NULL, false);
		for each (auto& file in files)
		{	
			if (_resourcesService->CheckIgnored(file) == false)
			{
				ResourceService::Type resType = _resourcesService->DetermineType(file);

				ImGui::PushID(resIdx);

				switch (resType)
				{
				case ResourceService::Type::TEXTURE: DrawTextureItem(resIdx, _resourcesService->GetTextureResourceByName(Utils::GetFilename(file, false))); break;
				case ResourceService::Type::SCENE: DrawSceneItem(resIdx, _resourcesService->GetSceneByName(Utils::GetFilename(file, false))); break;
				case ResourceService::Type::SCRIPT: DrawScriptItem(resIdx, _resourcesService->GetScriptByName(Utils::GetFilename(file, false))); break;
				case ResourceService::Type::SHADER:break;
				case ResourceService::Type::NONE:
				default:
					DrawUnknownItem(resIdx, Utils::GetFilename(file, false)); break;
					break;
				}

				ImGui::PopID();
				ImGui::NextColumn();
				resIdx++;
			}
		}
	}

	void AssetViewer::DrawTextureItem(uint32_t resIdx, std::shared_ptr<TextureRes> textureResource)
	{
		uint32_t id = textureResource->GetResourceID();
		ImGui::BeginGroup();
		if (ImGui::ImageButton((ImTextureID)(INT_PTR)textureResource->GetTexture()->GetHandle(), ImVec2(64, 64)))
		{
			/* Fire a change of selection event */
			ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Editor::Events::EventType::SelectedAssetChanged, std::make_shared<Events::SelectedAssetChangedArgs>(textureResource));
		}
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ASSET_DND_PAYLOAD", &id, sizeof(uint32_t));
			ImGui::Image((ImTextureID)(INT_PTR)textureResource->GetTexture()->GetHandle(), ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Pase")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) { OSR_TRACE("Texture Deleted"); };
			ImGui::Separator();

			if (ImGui::BeginMenu("Convert To ..."))
			{
				if (ImGui::MenuItem("Sprite Sheet"))
				{
					OSR_TRACE("Texture Convert To -> Sprite Scene");
					ImGui::OpenPopup("SpriteSheet?");
				};

				ImGui::EndMenu();
			};
			ImGui::EndPopup();
		}

		ImGui::SameLine(56.0f);
		ImGui::Icon(_TextureIcon, ImVec2(16, 16));
		ImGui::Text(textureResource->GetName().c_str());
		ImGui::EndGroup();
	}

	void AssetViewer::DrawSceneItem(uint32_t resIdx, std::shared_ptr<SceneRes> sceneResource)
	{
		ImGui::BeginGroup();
		if (ImGui::IconButton(_SceneIcon, 1, true, ImVec2(64, 64)))
		{
			ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->LoadScene(sceneResource->GetPath());
		}
		ImGui::Text(sceneResource->GetName().c_str());
		ImGui::EndGroup();
	}

	void AssetViewer::DrawScriptItem(uint32_t resIdx, std::shared_ptr<ScriptRes> scriptResource)
	{
		uint32_t id = scriptResource->GetResourceID();

		ImGui::BeginGroup();
		ImGui::IconButton(_ScriptIcon, 1, true, ImVec2(64, 64));
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ASSET_DND_PAYLOAD", &id, sizeof(uint32_t));
			ImGui::Icon(_ScriptIcon, ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Open") == true)
			{
				Utils::OpenFileWithSystem(scriptResource->GetPath());
			}

			ImGui::MenuItem("Rename");
			ImGui::MenuItem("Edit");
			ImGui::Separator();
			if(ImGui::MenuItem("Delete") == true)
			{
			}

			ImGui::EndPopup();
		}

		ImGui::Text(scriptResource->GetName().c_str());
		ImGui::EndGroup();
	}

	void AssetViewer::DrawUnknownItem(uint32_t resIdx, std::string& unknownResourceName)
	{
		ImGui::BeginGroup();
		if (ImGui::IconButton(_UnknownIcon, 1, true, ImVec2(64, 64)))
		{
			//ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->LoadScene(sceneResource->GetPath());
		}
		ImGui::Text(unknownResourceName.c_str());
		ImGui::EndGroup();
	}
}