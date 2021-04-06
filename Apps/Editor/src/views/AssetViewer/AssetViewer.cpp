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
#include "views/Dialogs/NewScriptDialog.h"
#include "views/Dialogs/NewSceneDialog.h"
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
		float itemGroupWidth = 100.0f;
		int itemColumnCnt = 4;
		float itemGroupPaddingX = 32.0f;
		float itemColumnWidth;
		float itemColumnHeight;
		int itemLabelShortCharLimit = 9;
	} layoutSettings;

	static uint32_t dirId = 1;
	std::shared_ptr<AssetViewer::DirectoryEntry_s> directoryTree;

	std::shared_ptr<AssetViewer::DirectoryEntry_s> FindDirectoryEntry(std::shared_ptr<AssetViewer::DirectoryEntry_s> root, uint32_t id);
	std::shared_ptr<AssetViewer::DirectoryEntry_s> FindDirectoryEntry(std::shared_ptr<AssetViewer::DirectoryEntry_s> root, const std::string& dir);

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

				SetCurrentSelectedDirectory(FindDirectoryEntry(directoryTree, 1));
			});

		_EventService->Subscribe(Events::EventType::AddFileEntry, [this](Events::EventArgs& args)
			{
				Events::AddFileEntryArgs& evtArgs = static_cast<Events::AddFileEntryArgs&>(args);
				
				/* find the directory entry */
				auto foundDir = FindDirectoryEntry(directoryTree, evtArgs.dir);

				/* save the currently selected dir */
				std::string originalSelected = _currentSelectedDir->dir;

				/* refresh the directory structure */
				foundDir->files.clear();
				std::map<UID, std::shared_ptr<Resource>> dirResources = _resourcesService->GetResourcesByDir(foundDir->dir);
				foundDir->files.insert(dirResources.begin(), dirResources.end());

				/* re-apply the directory entry to ensure the user selection is maintained */
				_currentSelectedDir = FindDirectoryEntry(directoryTree, originalSelected);
			});

		_EventService->Subscribe(Events::EventType::DeleteFileEntry, [this](Events::EventArgs& args)
			{
				Events::DeleteFileEntryArgs& evtArgs = static_cast<Events::DeleteFileEntryArgs&>(args);

				auto foundDir = FindDirectoryEntry(directoryTree, evtArgs.dir);

				if (evtArgs.isDir)
				{
					/* if we are deleting the current selected directory, we should move to the parent first */
					SetCurrentSelectedDirectory(foundDir);
				}

				/* save the currently selected dir */
				std::string originalSelected = _currentSelectedDir->dir;

				_refreshing = true;
				RefreshSubDir(std::string(foundDir->parent->dir), foundDir->parent);
				_refreshing = false;

				/* re-apply the directory entry to ensure the user selection is maintained */
				_currentSelectedDir = FindDirectoryEntry(directoryTree, originalSelected);
			});

		_EventService->Subscribe(Events::EventType::RenameFileEntry, [this](Events::EventArgs& args)
			{
				Events::RenameFileEntryArgs& evtArgs = static_cast<Events::RenameFileEntryArgs&>(args);

				if (evtArgs.isDir)
				{
					auto foundDir = FindDirectoryEntry(directoryTree, evtArgs.dir);

					/* save the currently selected dir */
					std::string originalSelected = _currentSelectedDir->dir;

					_refreshing = true;
					RefreshSubDir(std::string(foundDir->dir), foundDir);
					_refreshing = false;

					/* re-apply the directory entry to ensure the user selection is maintained */
					_currentSelectedDir = FindDirectoryEntry(directoryTree, originalSelected);

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
			layoutSettings.itemColumnHeight = layoutSettings.itemGroupWidth + ImGui::CalcTextSize("").y;
			layoutSettings.itemColumnCnt = (int)(ImGui::GetContentRegionAvail().x / layoutSettings.itemColumnWidth);

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			if (!_refreshing)
				DrawAssetsItems();

			ImGui::EndChildFrame();
		}

		/* Debugger Window */
		/*ImGui::Begin((_Name + "_Debug").c_str());
		ImGui::DragFloat("Layout - ItemGroupWidth", &layoutSettings.itemGroupWidth, 1.0f, 16, 256);
		ImGui::DragFloat("Layout - itemGroupPaddingX", &layoutSettings.itemGroupPaddingX, 1.0f, 0, 32);
		ImGui::DragInt("Layout - itemLabelShortCharLimit", &layoutSettings.itemLabelShortCharLimit, 1.0f, 0, 32);

		ImGui::Text("Read Only");
		ImGui::Text("Layout - ItemColumnCnt %d", layoutSettings.itemColumnCnt);
		ImGui::Text("Layout - ItemColumnWidth %d", layoutSettings.itemColumnWidth);
		ImGui::Text("Var - currentSelectedResource %d", _currentSelectedResourceUID);
		ImGui::Text("Layout - ItemColumnWidth %d", layoutSettings.itemColumnWidth);
		ImGui::End();*/
	}

	void AssetViewer::Refresh()
	{
		directoryTree = std::make_shared<AssetViewer::DirectoryEntry_s>();

		/* set the root information */
		directoryTree->id = dirId;
		directoryTree->name = "Assets";
		directoryTree->dir = Utils::GetAssetFolder();
		directoryTree->parent = nullptr;

		/* get all assets in base asset directory */
		std::map<UID, std::shared_ptr<Resource>> dirResources = _resourcesService->GetResourcesByDir(Utils::GetAssetFolder() + "\\");
		directoryTree->files.insert(dirResources.begin(), dirResources.end());

		/* increment the dirID to prevent a id clash with the asset folder */
		dirId++;

		for (const auto& e : std::filesystem::directory_iterator(directoryTree->dir))
		{
			if (Utils::FolderExists(e.path().string()))
			{
				/* recursively add the sub dirs */
				directoryTree->subdirs.push_back(RefreshSubDir(e.path().string(), directoryTree));
			}
		}
	}

	std::shared_ptr<AssetViewer::DirectoryEntry_s> AssetViewer::RefreshSubDir(const std::string& folder, std::shared_ptr<AssetViewer::DirectoryEntry_s> parent)
	{
		std::shared_ptr<AssetViewer::DirectoryEntry_s> entry = std::make_shared<AssetViewer::DirectoryEntry_s>();
		entry->id = dirId;
		entry->name = Utils::GetFilename(folder, true);
		entry->dir = folder;
		entry->parent = parent;

		/* get all assets in base asset directory */
		std::map<UID, std::shared_ptr<Resource>> dirResources = _resourcesService->GetResourcesByDir(folder);
		entry->files.insert(dirResources.begin(), dirResources.end());

		dirId++;

		/* retrieve all the folders */
		for (const auto& e : std::filesystem::directory_iterator(folder))
		{
			/* check if it's a directory, if so recurse */
			if (Utils::FolderExists(e.path().string()))
			{
				entry->subdirs.push_back(RefreshSubDir(e.path().string(), entry));
			}
		}

		return entry;
	}

	void AssetViewer::SetCurrentSelectedDirectory(std::shared_ptr<AssetViewer::DirectoryEntry_s> directory)
	{
		_currentSelectedDir = directory;
	}

	std::shared_ptr<AssetViewer::DirectoryEntry_s> AssetViewer::GetCurrentSelectedDirectory()
	{
		return _currentSelectedDir;
	}

	bool AssetViewer::DirectoryTreeViewRecursive(std::shared_ptr<AssetViewer::DirectoryEntry_s> dirEntry)
	{
		bool itemClicked = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

		/* set the selected flag if the index if current selected */
		if (_currentSelectedDir != nullptr && dirEntry->id == _currentSelectedDir->id)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		bool isOpen = false;
		if (editSettings.enabled == true && editSettings.newDirectoryMode == false && editSettings.id == dirEntry->id)
		{
			if (DrawEditNode())
			{
				std::filesystem::path p(dirEntry->dir);
				Utils::RenameFolder(dirEntry->dir, p.parent_path().parent_path().string() + "\\" + editSettings.text);
			}
		}
		else
		{
			isOpen = ImGui::TreeNodeEx((void*)(intptr_t)(dirEntry->id), baseFlags, dirEntry->name.c_str());
		}

		DrawDirectoryContextMenu(dirEntry->id);

		if (editSettings.enabled == true &&  editSettings.newDirectoryMode == true && editSettings.id == dirEntry->id)
		{
			if (DrawEditNode())
			{
				Utils::CreateFolder(dirEntry->dir + "\\" + editSettings.text);
			}
		}

		if (isOpen)
		{
			for (auto& child : dirEntry->subdirs)
			{
				itemClicked = DirectoryTreeViewRecursive(child);

				if (ImGui::IsItemClicked() && !itemClicked)
				{
					SetCurrentSelectedDirectory(child);
					itemClicked = true;
				}
			}
			
			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemClicked())
			{
				SetCurrentSelectedDirectory(dirEntry);
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
					auto dir = FindDirectoryEntry(directoryTree, nodeId);

					/* copy the file into the project */
					Utils::CopySingleFile(file.value(), dir->dir);
				}
			}

			if (ImGui::BeginMenu("New Asset"))
			{
				if (ImGui::MenuItem("Scene"))
				{
					/* find the directory entry from the id */
					auto dir = FindDirectoryEntry(directoryTree, nodeId);

					_dialogService->OpenDialog(std::make_shared<NewSceneDialog>(_EditorLayer, dir->dir));
				}

				if (ImGui::MenuItem("Script"))
				{
					/* find the directory entry from the id */
					auto dir = FindDirectoryEntry(directoryTree, nodeId);

					_dialogService->OpenDialog(std::make_shared<NewScriptDialog>(_EditorLayer, dir->dir));
				}
				ImGui::EndMenu();
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
						auto dir = FindDirectoryEntry(directoryTree, _currentContextNodeIdx);

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
				auto dir = FindDirectoryEntry(directoryTree, nodeId);

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

		std::shared_ptr<AssetViewer::DirectoryEntry_s> dir = GetCurrentSelectedDirectory();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		if (dir != nullptr)
		{
			if (ImGui::BeginTable("assetItemsTable", layoutSettings.itemColumnCnt, ImGuiTableFlags_SizingStretchProp))
			{
				uint32_t resIdx = 0;
				for (auto& res : dir->files)
				{	
					uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;

					if (tableCursor == 0)
					{
						ImGui::TableNextRow();
					}

					ImGui::TableSetColumnIndex(tableCursor);

					ImGui::BeginGroup();

					ImGui::PushID(&res);

					/* caculate the additional text size */
					ImVec2 textSize = ImGui::CalcTextSize(res.second->GetName().c_str());

					// DEBUG ONLY
					//ImGuiWindow* window = ImGui::GetCurrentWindowRead();
					//const ImVec2 p0 = window->DC.CursorPos;
					//ImGui::GetWindowDrawList()->AddRect(p0, p0 + ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnHeight), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));


					/* selectable interface */
					if (ImGui::Selectable("##title", _currentSelectedResourceUID == res.second->GetResourceID(), ImGuiSelectableFlags_SelectOnClick, ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemGroupWidth + textSize.y)))
					{
						_currentSelectedResourceUID = res.second->GetResourceID();
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - layoutSettings.itemColumnWidth);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((layoutSettings.itemColumnWidth / 2.0f) - (layoutSettings.itemGroupWidth / 2.0f)));

					ResourceType resType = static_cast<ResourceType>(res.second->GetType());

					switch (resType)
					{
					case ResourceType::TEXTURE:
						DrawTextureItem(resIdx, (TextureRes&)*res.second.get());
						break;
					case ResourceType::SCENE:
						DrawSceneItem(resIdx, (SceneRes&)*res.second.get());
						break;
					case ResourceType::SCRIPT:
						DrawScriptItem(resIdx, (ScriptRes&)*res.second.get());
						break;
					case ResourceType::SHADER:break;
					case ResourceType::NONE:
					default:
						break;
					}

					ImGui::PopID();
					ImGui::EndGroup();

					resIdx++;
				}
				
				ImGui::EndTable();
			}
		}
		ImGui::PopStyleVar();
	}

	void AssetViewer::DrawTextureItem(uint32_t resIdx, TextureRes& textureResource)
	{
		/* context menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) 
			{ 
				//_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
				//	[&](void* data) {
				//		Utils::RemoveFile(std::dynamic_pointer_cast<TextureRes> (_currentSelectedResource)->GetPath());
				//	});
			};

			ImGui::EndPopup();
		}

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("DND_TEXTURE", &textureResource.GetResourceID(), sizeof(UID));
			ImGui::Image((ImTextureID)(INT_PTR)textureResource.GetTexture()->GetHandle(), ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(textureResource.GetName().c_str());
			ImGui::Text("Filename: %s", textureResource.GetPath().c_str());
			ImGui::Text("UID: %s", textureResource.GetResourceID().str().c_str());
			ImGui::EndTooltip();
		}

		/* render widgets */
		ImGui::Image((ImTextureID)(INT_PTR)textureResource.GetTexture()->GetHandle(), ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::TextClipped(textureResource.GetName().c_str(), layoutSettings.itemGroupWidth, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");

	}

	void AssetViewer::DrawSceneItem(uint32_t resIdx, SceneRes& sceneResource)
	{
		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->LoadScene(sceneResource.GetPath());
		}

		/* context menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) 
			{ 
				//_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
				//	[&](void* data) {
				//		Utils::RemoveFile(std::dynamic_pointer_cast<TextureRes> (_currentSelectedResource)->GetPath());
				//	});
			};

			ImGui::EndPopup();
		}

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("DND_SCENE", &sceneResource.GetResourceID(), sizeof(UID));
			ImGui::Image(*_SceneIcon, ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(sceneResource.GetName().c_str());
			ImGui::Text("Filename: %s", sceneResource.GetPath().c_str());
			ImGui::Text("UID: %s", sceneResource.GetResourceID().str().c_str());
			ImGui::EndTooltip();
		}

		/* render widgets */
		ImGui::Image(*_SceneIcon, ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::TextClipped(sceneResource.GetName().c_str(), layoutSettings.itemGroupWidth, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");

	}

	void AssetViewer::DrawScriptItem(uint32_t resIdx, ScriptRes& scriptResource)
	{
		/* context menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Copy")) { OSR_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { OSR_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { OSR_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Delete"))
			{
				//_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
				//	[&](void* data) {
				//		Utils::RemoveFile(std::dynamic_pointer_cast<TextureRes> (_currentSelectedResource)->GetPath());
				//	});
			};

			ImGui::EndPopup();
		}

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("DND_SCRIPT", &scriptResource.GetResourceID(), sizeof(UID));
			ImGui::Image(*_ScriptIcon, ImVec2(32, 32));
			ImGui::EndDragDropSource();
		}

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(scriptResource.GetName().c_str());
			ImGui::Text("Filename: %s", scriptResource.GetPath().c_str());
			ImGui::Text("UID: %s", scriptResource.GetResourceID().str().c_str());
			ImGui::EndTooltip();
		}
		
		ImGui::Image(*_ScriptIcon, ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
		ImGui::TextClipped(scriptResource.GetName().c_str(), layoutSettings.itemGroupWidth, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");
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

	std::shared_ptr<AssetViewer::DirectoryEntry_s> FindDirectoryEntry(std::shared_ptr<AssetViewer::DirectoryEntry_s> root, uint32_t id)
	{
		if (root->id == id)
			return root;

		for (auto& entry : root->subdirs)
		{
			auto foundItem = FindDirectoryEntry(entry, id);
			if (foundItem != nullptr)
			{
				return foundItem;
			}
		}

		return nullptr;
	}

	std::shared_ptr<AssetViewer::DirectoryEntry_s> FindDirectoryEntry(std::shared_ptr<AssetViewer::DirectoryEntry_s> root, const std::string& dir)
	{
		if (root->dir == dir)
			return root;

		for (auto& entry : root->subdirs)
		{
			auto foundItem = FindDirectoryEntry(entry, dir);
			if (foundItem != nullptr)
			{
				return foundItem;
			}
		}

		return nullptr;
	}
}