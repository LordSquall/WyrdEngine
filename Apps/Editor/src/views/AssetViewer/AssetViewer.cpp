#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/KeyCodes.h>
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
	struct EditSettings_s
	{
		bool enabled = false;
		bool newDirectoryMode = false;
		std::string text;
		uint32_t id;
	} editSettings;

	struct LayoutSettings_s
	{
		int itemGroupWidth = 64;
		int itemColumnCnt = 4;
		int itemGroupPaddingX = 6;
		int itemColumnWidth;
		int itemLabelShortCharLimit = 9;
	} layoutSettings;

	static uint32_t dirId = 1;
	AssetViewer::DirectoryEntry_s directoryTree;

	AssetViewer::DirectoryEntry_s* FindDirectoryEntry(AssetViewer::DirectoryEntry_s* root, uint32_t id);
	AssetViewer::DirectoryEntry_s* FindDirectoryEntry(AssetViewer::DirectoryEntry_s* root, const std::string& dir);

	AssetViewer::AssetViewer(EditorLayer* editorLayer) : EditorViewBase("Asset Viewer", editorLayer), _currentContextNodeIdx(0), _currentSelectedDir(nullptr), _refreshing(false)
	{
		/* cache the service(s) */
		_resourcesService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_dialogService = ServiceManager::Get<DialogService>(ServiceManager::Service::Dialog);

		/* cache the icon pointers */
		_UnknownIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_unknown");
		_SceneIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_scene");
		_TextureIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_texture");
		_ScriptIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_script");

		/* register from events */
		_EventService->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args)
			{
				Refresh();

				SetCurrentSelectedDirectory(FindDirectoryEntry(&directoryTree, 1));
			});

		_EventService->Subscribe(Events::EventType::AddFileEntry, [this](Events::EventArgs& args)
			{
				Events::AddFileEntryArgs& evtArgs = static_cast<Events::AddFileEntryArgs&>(args);
				
				/* find the directory entry */
				AssetViewer::DirectoryEntry_s* foundDir = FindDirectoryEntry(&directoryTree, evtArgs.dir);

				/* save the currently selected dir */
				std::string originalSelected = _currentSelectedDir->dir;

				/* refresh the directory structure */
				_refreshing = true;
				RefreshSubDir(std::string(foundDir->parent->dir), *foundDir->parent);
				_refreshing = false;

				/* re-apply the directory entry to ensure the user selection is maintained */
				_currentSelectedDir = FindDirectoryEntry(&directoryTree, originalSelected);

			});

		_EventService->Subscribe(Events::EventType::DeleteFileEntry, [this](Events::EventArgs& args)
			{
				Events::DeleteFileEntryArgs& evtArgs = static_cast<Events::DeleteFileEntryArgs&>(args);

				AssetViewer::DirectoryEntry_s* foundDir = FindDirectoryEntry(&directoryTree, evtArgs.dir);

				if (evtArgs.isDir)
				{
					/* if we are deleting the current selected directory, we should move to the parent first */
					SetCurrentSelectedDirectory(foundDir);
				}

				/* save the currently selected dir */
				std::string originalSelected = _currentSelectedDir->dir;

				_refreshing = true;
				RefreshSubDir(std::string(foundDir->parent->dir), *foundDir->parent);
				_refreshing = false;

				/* re-apply the directory entry to ensure the user selection is maintained */
				_currentSelectedDir = FindDirectoryEntry(&directoryTree, originalSelected);
			});

		_EventService->Subscribe(Events::EventType::RenameFileEntry, [this](Events::EventArgs& args)
			{
				Events::RenameFileEntryArgs& evtArgs = static_cast<Events::RenameFileEntryArgs&>(args);

				if (evtArgs.isDir)
				{
					AssetViewer::DirectoryEntry_s* foundDir = FindDirectoryEntry(&directoryTree, evtArgs.dir);

					/* save the currently selected dir */
					std::string originalSelected = _currentSelectedDir->dir;

					_refreshing = true;
					RefreshSubDir(std::string(foundDir->dir), *foundDir);
					_refreshing = false;

					/* re-apply the directory entry to ensure the user selection is maintained */
					_currentSelectedDir = FindDirectoryEntry(&directoryTree, originalSelected);

					OSR_TRACE("Folder Renamed!");
				}
			});
	}

	void AssetViewer::OnEditorRender()
	{
		float navigationPanelWidth = 150.0f;
		float itemGroupWidth = 64.0f;
		static ResourceType selectedType = ResourceType::NONE;


		if (_workspaceService->GetCurrentProject() != nullptr)
		{
			ImGui::BeginChildFrame(1, ImVec2(navigationPanelWidth, ImGui::GetContentRegionAvail().y));

			DirectoryTreeViewRecursive(directoryTree);

			ImGui::EndChildFrame();

			ImGui::SameLine();

			/* capture the width of the child area to determine the column count */
			layoutSettings.itemColumnWidth = layoutSettings.itemGroupWidth + (layoutSettings.itemGroupPaddingX * 2);
			layoutSettings.itemColumnCnt = ImGui::GetContentRegionAvail().x / layoutSettings.itemColumnWidth;

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			if (!_refreshing)
				DrawAssetsItems();

			ImGui::EndChildFrame();
		}

		/* Debugger Window */
		/*ImGui::Begin((_Name + "_Debug").c_str());
		ImGui::DragInt("Layout - ItemGroupWidth", &layoutSettings.itemGroupWidth, 1.0f, 16, 256);
		ImGui::DragInt("Layout - itemGroupPaddingX", &layoutSettings.itemGroupPaddingX, 1.0f, 0, 32);
		ImGui::DragInt("Layout - itemLabelShortCharLimit", &layoutSettings.itemLabelShortCharLimit, 1.0f, 0, 32);

		ImGui::Text("Read Only");
		ImGui::Text("Layout - ItemColumnCnt %d", layoutSettings.itemColumnCnt);
		ImGui::Text("Layout - ItemColumnWidth %d", layoutSettings.itemColumnWidth);
		ImGui::Text("Var - currentSelectedResource %d", _currentSelectedResource != nullptr ? _currentSelectedResource->GetResourceID() : 0);
		ImGui::Text("Layout - ItemColumnWidth %d", layoutSettings.itemColumnWidth);
		ImGui::End();*/
	}

	void AssetViewer::Refresh()
	{
		/* clear previous data */
		directoryTree.subdirs.clear();

		/* set the root information */
		directoryTree.id = dirId;
		directoryTree.name = "Assets";
		directoryTree.dir = Utils::GetAssetFolder() + "\\";
		directoryTree.parent = nullptr;

		/* get all assets in base asset directory */
		std::map<UID, std::shared_ptr<Resource>> dirResources = _resourcesService->GetResourcesByDir(Utils::GetAssetFolder() + "\\");
		directoryTree.files.insert(dirResources.begin(), dirResources.end());

		/* increment the dirID to prevent a id clash with the asset folder */
		dirId++;

		/* recursively add the sub dirs */
		RefreshSubDir(Utils::GetAssetFolder(), directoryTree);
	}

	void AssetViewer::RefreshSubDir(const std::string& folder, AssetViewer::DirectoryEntry_s& dirEntry)
	{
		/* clear previous data */
		dirEntry.subdirs.clear();
		dirEntry.files.clear();

		if (Utils::FolderExists(folder))
		{
			for (const auto& entry : std::filesystem::directory_iterator(folder))
			{
				std::string name = Utils::GetFilename(entry.path().string(), true);

				if (Utils::FolderExists(entry.path().string()))
				{
					AssetViewer::DirectoryEntry_s newEntry;

					RefreshSubDir(entry.path().string(), newEntry);

					/* get all assets in base asset directory */
					std::map<UID, std::shared_ptr<Resource>> dirResources = _resourcesService->GetResourcesByDir(entry.path().string() + "\\");
					newEntry.files.insert(dirResources.begin(), dirResources.end());

					newEntry.id = dirId++;
					newEntry.name = name;
					newEntry.dir = entry.path().string() + "\\";
					newEntry.parent = &dirEntry;
					dirEntry.subdirs[newEntry.id] = newEntry;
				}
			}
		}
	}

	void AssetViewer::SetCurrentSelectedDirectory(AssetViewer::DirectoryEntry_s* directory)
	{
		_currentSelectedDir = directory;
	}

	AssetViewer::DirectoryEntry_s* AssetViewer::GetCurrentSelectedDirectory()
	{
		return _currentSelectedDir;
	}

	bool AssetViewer::DirectoryTreeViewRecursive(AssetViewer::DirectoryEntry_s& dirEntry)
	{
		bool itemClicked = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

		/* set the selected flag if the index if current selected */
		if (_currentSelectedDir != nullptr && dirEntry.id == _currentSelectedDir->id)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool isOpen = false;
		if (editSettings.enabled == true && editSettings.newDirectoryMode == false && editSettings.id == dirEntry.id)
		{
			if (DrawEditNode())
			{
				std::filesystem::path p(dirEntry.dir);
				Utils::RenameFolder(dirEntry.dir, p.parent_path().parent_path().string() + "\\" + editSettings.text);
			}
		}
		else
		{
			isOpen = ImGui::TreeNodeEx((void*)(intptr_t)(dirEntry.id), baseFlags, dirEntry.name.c_str());
		}

		DrawDirectoryContextMenu(dirEntry.id);

		if (editSettings.enabled == true &&  editSettings.newDirectoryMode == true && editSettings.id == dirEntry.id)
		{
			if (DrawEditNode())
			{
				Utils::CreateFolder(dirEntry.dir + "\\" + editSettings.text);
			}
		}

		if (isOpen)
		{
			for (auto& children : dirEntry.subdirs)
			{
				itemClicked = DirectoryTreeViewRecursive(children.second);

				if (ImGui::IsItemClicked() && !itemClicked)
				{
					SetCurrentSelectedDirectory(&children.second);
					itemClicked = true;
				}
			}
			
			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemClicked())
			{
				SetCurrentSelectedDirectory(&dirEntry);
			}
		}

		return itemClicked;
	}

	void AssetViewer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(AssetViewer::OnKeyPressedEvent));
	}

	bool AssetViewer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == OSR_KEY_F2)
		{
			editSettings.enabled = true;
			editSettings.newDirectoryMode = false;
			editSettings.id = _currentSelectedDir->id;
			editSettings.text = "New File Name";
		}
		return true;
	}

	void AssetViewer::DrawDirectoryContextMenu(uint32_t nodeId)
	{
		if (ImGui::BeginPopupContextItem())
		{
			_currentContextNodeIdx = nodeId;

			if (ImGui::MenuItem("Import Asset"))
			{
				std::optional<std::string> file = Utils::OpenFile({ { "All Files (*)", "*.*" } });
				if (file)
				{
					/* find the directory entry from the id */
					DirectoryEntry_s* dir = FindDirectoryEntry(&directoryTree, nodeId);

					/* copy the file into the project */
					Utils::CopySingleFile(file.value(), dir->dir);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("New Folder")) 
			{
				/* setup properties to allow edit */
				editSettings.enabled = true;
				editSettings.newDirectoryMode = true;
				editSettings.id = nodeId;
				editSettings.text = "New Folder";
			}

			if (ImGui::MenuItem("Delete", nullptr, nullptr, nodeId != 1))
			{
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
					[&](void* data) {

						/* find the directory entry from the id */
						DirectoryEntry_s* dir = FindDirectoryEntry(&directoryTree, _currentContextNodeIdx);

						/* delete the folder */
						Utils::DeleteFolder(dir->dir);
					},
					[&](void* data) {
						OSR_TRACE("Failure Callback Hit!!");
					});
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open in Explorer"))
			{
				/* find the directory entry from the id */
				DirectoryEntry_s* dir = FindDirectoryEntry(&directoryTree, nodeId);

				ShellExecuteA(NULL, "open", dir->dir.c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			ImGui::EndPopup();
		}
	}

	bool AssetViewer::DrawEditNode()
	{
		ImGui::PushItemWidth(-1);
		ImGui::SetKeyboardFocusHere();
		if (ImGui::InputText("##label", &editSettings.text, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			editSettings.enabled = false;
			return true;
		}
		ImGui::PopItemWidth();

		return false;
	}

	void AssetViewer::DrawAssetsItems()
	{
		uint32_t resIdx = 0;

		DirectoryEntry_s* dir = GetCurrentSelectedDirectory();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		if (dir != nullptr)
		{
			ImGui::Columns(layoutSettings.itemColumnCnt, NULL, false);
			for (auto& res : dir->files)
			{
				ResourceType resType = static_cast<ResourceType>(res.second->GetType());

				ImGui::PushID(resIdx);
				ImGui::BeginGroup();

				switch (resType)
				{
				case ResourceType::TEXTURE:
					DrawTextureItem(resIdx, dynamic_pointer_cast<TextureRes>(res.second));
					break;
				case ResourceType::SCENE:
					DrawSceneItem(resIdx, dynamic_pointer_cast<SceneRes>(res.second));
					break;
				case ResourceType::SCRIPT:
					DrawScriptItem(resIdx, dynamic_pointer_cast<ScriptRes>(res.second));
					break;
				case ResourceType::SHADER:break;
				case ResourceType::NONE:
				default:
					break;
				}

				ImGui::EndGroup();
				ImGui::PopID();
				ImGui::NextColumn();
				resIdx++;
			}
			ImGui::Columns(1);
		}
		ImGui::PopStyleVar();
	}

	void AssetViewer::DrawTextureItem(uint32_t resIdx, std::shared_ptr<TextureRes> textureResource)
	{
		/* calculate the total height of the item */
		ImVec2 labelSize = ImGui::CalcTextSize(textureResource->GetName().c_str());
		ImVec2 groupSize = ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnWidth + labelSize.y);

		if (ImGui::Selectable("##title", _currentSelectedResource != nullptr ? _currentSelectedResource->GetResourceID() == textureResource->GetResourceID() : false, ImGuiSelectableFlags_SelectOnClick, groupSize))
		{
			_currentSelectedResource = textureResource;
		}

		if (ImGui::BeginPopupContextItem())
		{
			_currentSelectedResource = textureResource;

			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) 
			{ 
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
					[&](void* data) {
						Utils::RemoveFile(std::dynamic_pointer_cast<TextureRes> (_currentSelectedResource)->GetPath());
					});
			};

			ImGui::EndPopup();
		}
	
		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			//std::shared_ptr<Osiris::Texture> texture = textureResource->GetTexture();
			ImGui::SetDragDropPayload("TEXTURE_ASSET_PAYLOAD", &textureResource, sizeof(std::shared_ptr<TextureRes>));
			ImGui::Image((ImTextureID)(INT_PTR)textureResource->GetTexture()->GetHandle(), ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(textureResource->GetName().c_str());
			ImGui::Text("Filename: %s", textureResource->GetPath().c_str());
			ImGui::Text("UID: %s", textureResource->GetResourceID().str().c_str());
			ImGui::EndTooltip();
		}

		/* reset the cursor */
		ImGui::SameLine();
		ImGui::SetCursorPosX((ImGui::GetCursorPosX() - groupSize.x) + layoutSettings.itemGroupPaddingX);

		ImGui::Image((ImTextureID)(INT_PTR)textureResource->GetTexture()->GetHandle(), ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2, ImGui::GetCursorPosY() - labelSize.y));

		if (labelSize.x > (layoutSettings.itemColumnWidth - 4))
		{
			ImGui::Text("%.*s...", layoutSettings.itemLabelShortCharLimit, textureResource->GetName().c_str());
		}
		else
		{
			ImGui::Text(textureResource->GetName().c_str());
		}
	}

	void AssetViewer::DrawSceneItem(uint32_t resIdx, std::shared_ptr<SceneRes> sceneResource)
	{
		/* calculate the total height of the item */
		ImVec2 labelSize = ImGui::CalcTextSize(sceneResource->GetName().c_str());
		ImVec2 groupSize = ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnWidth + labelSize.y);

		if (ImGui::Selectable("##title", _currentSelectedResource != nullptr ? _currentSelectedResource->GetResourceID() == sceneResource->GetResourceID() : false, ImGuiSelectableFlags_SelectOnClick, groupSize))
		{
			_currentSelectedResource = sceneResource;
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to load scene?",
					[&](void* data) {
						_workspaceService->LoadScene(dynamic_pointer_cast<SceneRes>(_currentSelectedResource)->GetPath());
					});
			}
		}

		if (ImGui::BeginPopupContextItem())
		{
			_currentSelectedResource = sceneResource;

			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) 
			{ 
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
					[&](void* data) {
						Utils::RemoveFile(std::dynamic_pointer_cast<TextureRes> (_currentSelectedResource)->GetPath());
					});
			};
			
			ImGui::Separator();
			if (ImGui::MenuItem("Open in external editor"))
			{
				Utils::OpenFileWithSystem(sceneResource->GetPath());
			}

			ImGui::EndPopup();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(sceneResource->GetName().c_str());
			ImGui::EndTooltip();
		}

		/* reset the cursor */
		ImGui::SameLine();
		ImGui::SetCursorPosX((ImGui::GetCursorPosX() - groupSize.x) + layoutSettings.itemGroupPaddingX);

		ImGui::Image(_SceneIcon, ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2, ImGui::GetCursorPosY() - labelSize.y));

		if (labelSize.x > (layoutSettings.itemColumnWidth - 4))
		{
			ImGui::Text("%.*s...", layoutSettings.itemLabelShortCharLimit, sceneResource->GetName().c_str());
		}
		else
		{
			ImGui::Text(sceneResource->GetName().c_str());
		}
	}

	void AssetViewer::DrawScriptItem(uint32_t resIdx, std::shared_ptr<ScriptRes> scriptResource)
	{
		/* calculate the total height of the item */
		ImVec2 labelSize = ImGui::CalcTextSize(scriptResource->GetName().c_str());
		ImVec2 groupSize = ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnWidth + labelSize.y);

		if (ImGui::Selectable("##title", _currentSelectedResource != nullptr ? _currentSelectedResource->GetResourceID() == scriptResource->GetResourceID() : false, ImGuiSelectableFlags_SelectOnClick, groupSize))
		{
			_currentSelectedResource = scriptResource;
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{

			}
		}

		if (ImGui::BeginPopupContextItem())
		{
			_currentSelectedResource = scriptResource;

			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete"))
			{
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?",
					[&](void* data) {
						Utils::RemoveFile(std::dynamic_pointer_cast<ScriptRes> (_currentSelectedResource)->GetPath());
					});
			};

			ImGui::Separator();
			if (ImGui::MenuItem("Open in external editor"))
			{
				Utils::OpenFileWithSystem(scriptResource->GetPath());
			}

			ImGui::EndPopup();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(scriptResource->GetName().c_str());
			ImGui::EndTooltip();
		}

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("SCRIPT_ASSET_PAYLOAD", &scriptResource, sizeof(std::shared_ptr<ScriptRes>));
			ImGui::Image(_ScriptIcon, ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		/* reset the cursor */
		ImGui::SameLine();
		ImGui::SetCursorPosX((ImGui::GetCursorPosX() - groupSize.x) + layoutSettings.itemGroupPaddingX);

		ImGui::Image(_ScriptIcon, ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 2, ImGui::GetCursorPosY() - labelSize.y));

		if (labelSize.x > (layoutSettings.itemColumnWidth - 4))
		{
			ImGui::Text("%.*s...", layoutSettings.itemLabelShortCharLimit, scriptResource->GetName().c_str());
		}
		else
		{
			ImGui::Text(scriptResource->GetName().c_str());
		}
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

	AssetViewer::DirectoryEntry_s* FindDirectoryEntry(AssetViewer::DirectoryEntry_s* root, uint32_t id)
	{
		if (root->id == id)
			return root;

		for (auto& entry : root->subdirs)
		{
			AssetViewer::DirectoryEntry_s* foundItem = FindDirectoryEntry(&entry.second, id);
			if (foundItem != nullptr)
			{
				return foundItem;
			}
		}

		return nullptr;
	}

	AssetViewer::DirectoryEntry_s* FindDirectoryEntry(AssetViewer::DirectoryEntry_s* root, const std::string& dir)
	{
		if (root->dir == dir)
			return root;

		for (auto& entry : root->subdirs)
		{
			AssetViewer::DirectoryEntry_s* foundItem = FindDirectoryEntry(&entry.second, dir);
			if (foundItem != nullptr)
			{
				return foundItem;
			}
		}

		return nullptr;
	}
}