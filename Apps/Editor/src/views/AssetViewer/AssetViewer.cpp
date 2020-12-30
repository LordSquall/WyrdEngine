#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>

/* local includes */
#include "AssetViewer.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

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
		float navigationPanelWidth = 150.0f;
		float itemGroupWidth = 64.0f;
		static ResourceFactory::Type selectedType = ResourceFactory::NONE;

		if (_workspaceService->GetCurrentProject() != nullptr)
		{
			
			ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y));

			uint32_t count = 1;
			for (const auto& entry : std::filesystem::recursive_directory_iterator((Utils::GetAssetFolder())))
				count++;

			static int selection_mask = 0;

			auto clickState = DirectoryTreeViewRecursive(Utils::GetAssetFolder(), &count, &selection_mask);

			if (clickState.first)
			{
				_currentDir = clickState.second;
			}

			ImGui::EndChildFrame();

			ImGui::SameLine();

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			DrawAssetsItems();
		
			ImGui::EndChildFrame();
		}
	}

#define BIT(x) (1 << x)

	std::pair<bool, const std::string> AssetViewer::DirectoryTreeViewRecursive(const std::filesystem::path& path, uint32_t* count, int* selection_mask, int depth)
	{
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

		bool any_node_clicked = false;
		std::string node_clicked;

		if (depth == 0)
		{
			ImGuiTreeNodeFlags rootNodeFlags = baseFlags | ImGuiTreeNodeFlags_Leaf;
			/*const bool is_selected = (*selection_mask & BIT(*count)) != 0;
			if (is_selected)
				rootNodeFlags |= ImGuiTreeNodeFlags_Selected;*/

			ImGui::TreeNodeEx("_root", rootNodeFlags, "Assets");

			if (ImGui::IsItemClicked())
			{
				node_clicked = path.string();
				any_node_clicked = true;
			}

			DrawDirectoryContextMenu(path.string());
		}

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			ImGuiTreeNodeFlags nodeFlags = baseFlags;
			const bool is_selected = (*selection_mask & BIT(*count)) != 0;
			if (is_selected)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			std::string name = entry.path().string();

			auto lastSlash = name.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			name = name.substr(lastSlash, name.size() - lastSlash);

			bool entryIsFile = !std::filesystem::is_directory(entry.path());

			bool node_open = false;
			if (!entryIsFile)
			{
				node_open = ImGui::TreeNodeEx((void*)(intptr_t)(*count), nodeFlags, name.c_str());

				if (ImGui::IsItemClicked())
				{
					node_clicked = entry.path().string();
					any_node_clicked = true;
				}

				DrawDirectoryContextMenu(entry.path().string());
			}

			(*count)--;

			if (!entryIsFile)
			{
				if (node_open)
				{

					auto clickState = DirectoryTreeViewRecursive(entry.path(), count, selection_mask, depth + 1);

					if (!any_node_clicked)
					{
						any_node_clicked = clickState.first;
						node_clicked = clickState.second;
					}

					ImGui::TreePop();
				}
			}
		}

		if (depth == 0)
		{
			ImGui::TreePop();
		}

		return { any_node_clicked, node_clicked };
	}

	void AssetViewer::DrawDirectoryContextMenu(const std::string& dir)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::BeginMenu("New Asset"))
			{
				ImGui::MenuItem("Scene", nullptr, nullptr, false);
				ImGui::MenuItem("Texture", nullptr, nullptr, false);
				ImGui::MenuItem("Shader", nullptr, nullptr, false);
				ImGui::MenuItem("Script", nullptr, nullptr, false);
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Import Asset"))
			{
				std::optional<std::string> file = Utils::OpenFile({ { "All Files (*)", "*.*" } });
				if (file)
				{
					// copy the file into the project
					Utils::CopySingleFile(file.value(), dir);
				}
			}

			if (ImGui::MenuItem("New Folder")) 
			{
			
			}

			ImGui::EndPopup();
		}
	}


	void AssetViewer::DrawAssetsItems()
	{
		uint32_t resIdx = 0;
		uint32_t rowCnt = 0;
		uint32_t colCnt = 4;
		std::vector<std::string> files = Utils::GetFileList(_currentDir, false);

		ImGui::Columns(colCnt, NULL, false); 
		for(auto& file : files)
		{	
			if (_resourcesService->CheckIgnored(file) == false)
			{
				ResourceFactory::Type resType = _resourcesService->DetermineType(file);

				ImGui::PushID(resIdx);

				switch (resType)
				{
				case ResourceFactory::TEXTURE: 
					DrawTextureItem(resIdx, _resourcesService->GetResourceByName<TextureRes>(ResourceFactory::TEXTURE, Utils::GetFilename(file, false))); 
					break;
				/**case ResourceFactory::SCENE: DrawSceneItem(resIdx, _resourcesService->GetSceneByName(Utils::GetFilename(file, false))); break;
				case ResourceFactory::SCRIPT: DrawScriptItem(resIdx, _resourcesService->GetScriptByName(Utils::GetFilename(file, false))); break;*/
				case ResourceFactory::SHADER:break;
				case ResourceFactory::NONE:
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
			std::shared_ptr<Osiris::Texture> texture = textureResource->GetTexture();
			ImGui::SetDragDropPayload("TEXTURE_ASSET_PAYLOAD", &texture, sizeof(std::shared_ptr<Osiris::Texture>));
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

		if (ImGui::BeginPopupContextItem("AssetTest"))
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