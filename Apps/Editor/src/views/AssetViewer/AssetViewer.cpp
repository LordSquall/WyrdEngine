#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
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

namespace Wyrd::Editor
{
	/**
	* Structure representing the current state of the Edit Directory mechanism
	*/
	struct EditDirectoryState_s
	{
		bool newDirectoryMode = false;
		std::string path;
		std::string text;
	} editDirectoryState;

	struct LayoutSettings_s
	{
		float itemGroupWidth = 50.0f;
		int itemColumnCnt = 4;
		float itemGroupPaddingX = 32.0f;
		float itemColumnWidth;
		float itemColumnHeight;
		int itemLabelShortCharLimit = 9;
	} layoutSettings;

	AssetViewer::AssetViewer(EditorLayer* editorLayer) : EditorViewBase("Asset Viewer", editorLayer), _SelectedDirectory(""), _SelectedResource(nullptr), _DeleteDirectoryState(""), _DeleteAssetState("")
	{
		/* cache the service(s) */
		_resourcesService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_settingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
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
				// TODO
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

			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 6.0f);
			DirectoryTreeViewRecursive(Utils::GetAssetFolder());
			ImGui::PopStyleVar();

			ImGui::EndChildFrame();

			ImGui::SameLine();

			/* capture the width of the child area to determine the column count */
			layoutSettings.itemColumnWidth = layoutSettings.itemGroupWidth + (layoutSettings.itemGroupPaddingX * 2);
			layoutSettings.itemColumnHeight = layoutSettings.itemGroupWidth + ImGui::CalcTextSize("").y;
			layoutSettings.itemColumnCnt = (int)(ImGui::GetContentRegionAvail().x / layoutSettings.itemColumnWidth);

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

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

	bool AssetViewer::DirectoryTreeViewRecursive(const std::string& path)
	{
		/* we don't display any files in the directory tree */
		if (std::filesystem::is_directory(path))
		{
			bool itemClicked = false;
			ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

			/* set the selected flag if the index if current selected */
			if (_SelectedDirectory.compare(path) == 0)
			{
				baseFlags |= ImGuiTreeNodeFlags_Selected;
			}

			/* set the selected flag if the index if current selected */
			int subDirCount = Utils::CountSubDirectories(path);
			if (subDirCount == 0)
			{
				baseFlags |= ImGuiTreeNodeFlags_Leaf;
			}

			bool isOpen = false;
			
			/* create directory name */
			auto lastSlash = path.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			std::string name = path.substr(lastSlash, path.size() - lastSlash);

			/* draw tree node */
			isOpen = ImGui::TreeNodeEx(path.c_str(), baseFlags, name.c_str());
		
			if (ImGui::IsItemClicked())
			{
				_SelectedDirectory = path;
				itemClicked = true;
			}


			/* if we are in edit mode we want to create an edit node as the first element in the tree to allow the new folder creation */
			if (path == editDirectoryState.path)
			{
				if (DrawEditDirectoryNode() == true)
				{
					Utils::CreateFolder(path + "\\" + editDirectoryState.text);
					editDirectoryState.path = "";
				}
			}

			DrawDirectoryContextMenu(path);

			if (isOpen)
			{
				for (const auto& entry : std::filesystem::directory_iterator(path))
				{
					itemClicked = DirectoryTreeViewRecursive(entry.path().string());

					/*if (ImGui::IsItemClicked() && !itemClicked)
					{
						SetCurrentSelectedDirectory(entry.path().string());
						itemClicked = true;
					}*/
				}

				ImGui::TreePop();
			}
			else
			{
				if (ImGui::IsItemClicked())
				{
					//SetCurrentSelectedDirectory(dirEntry);
				}
			}


			return itemClicked;
		}
		return false;
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
			//editSettings.enabled = true;
			//editSettings.newDirectoryMode = false;
			//editSettings.id = _currentSelectedDir->id;
			//editSettings.text = "New File Name";
		}
		return true;
	}

	void AssetViewer::DrawDirectoryContextMenu(const std::string& path)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Import Asset"))
			{
				std::optional<std::string> file = Utils::OpenFile({ { "All Files (*)", "*.*" } });
				if (file)
				{
					/* copy the file into the project */
					Utils::CopySingleFile(file.value(), path);
				}
			}

			if (ImGui::BeginMenu("New Asset"))
			{
				if (ImGui::MenuItem("Scene"))
				{
					_dialogService->OpenDialog(std::make_shared<NewSceneDialog>(_EditorLayer, path));
				}

				if (ImGui::MenuItem("Script"))
				{
					_dialogService->OpenDialog(std::make_shared<NewScriptDialog>(_EditorLayer, path));
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("New Folder")) 
			{
				/* setup properties to allow edit */
				editDirectoryState.path = path;
				editDirectoryState.text = "New Folder";
			}

			if (ImGui::MenuItem("Delete"))
			{
				_DeleteDirectoryState = path;
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?",
					[&](void* data) {
						/* delete the folder */
						Utils::DeleteFolder(_DeleteDirectoryState);
						_DeleteDirectoryState = "";
					},
					[&](void* data) {
						_DeleteDirectoryState = "";
					});
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Open in Explorer"))
			{
				ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			ImGui::EndPopup();
		}
	}

	bool AssetViewer::DrawEditDirectoryNode()
	{
		ImGui::PushItemWidth(-1);
		ImGui::SetKeyboardFocusHere();
		if (ImGui::InputText("##label", &editDirectoryState.text, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			return true;
		}
		ImGui::PopItemWidth();

		return false;
	}

	void AssetViewer::DrawAssetsItems()
	{
		uint32_t resIdx = 0;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		if (_SelectedDirectory.compare("") != 0)
		{
			if (ImGui::BeginTable("assetItemsTable", layoutSettings.itemColumnCnt, ImGuiTableFlags_SizingStretchProp))
			{
				uint32_t resIdx = 0;

				for (const auto& entry : std::filesystem::directory_iterator(_SelectedDirectory))
				{
					/* we only care about files */
					if (std::filesystem::is_regular_file(entry.path().string()))
					{
						/* retrieve matching resource cache entry */
						auto& resource = _resourcesService->GetResourceByFilePath(entry.path().string());

						if (resource == nullptr)
						{
							WYRD_CORE_TRACE("Resource Not Found: {0}", entry.path().string());
						}

						/* create file name */
						std::string fullName = entry.path().string();
							auto lastSlash = fullName.find_last_of("/\\");
							lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
							std::string name = fullName.substr(lastSlash, fullName.size() - lastSlash);


							uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;

							if (tableCursor == 0)
							{
								ImGui::TableNextRow();
							}

						ImGui::TableSetColumnIndex(tableCursor);

						ImGui::BeginGroup();

						ImGui::PushID(entry.path().string().c_str());

						/* caculate the additional text size */
						ImVec2 textSize = ImGui::CalcTextSize(name.c_str());

						// DEBUG ONLY
						//ImGuiWindow* window = ImGui::GetCurrentWindowRead();
						//const ImVec2 p0 = window->DC.CursorPos;
						//ImGui::GetWindowDrawList()->AddRect(p0, p0 + ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnHeight), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));


						/* selectable interface */
						bool selected = _SelectedResource == resource.get();
						if (ImGui::Selectable("##title", selected, ImGuiSelectableFlags_SelectOnClick, ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemGroupWidth + textSize.y)))
						{
							_SelectedResource = resource.get();
						}

						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() - layoutSettings.itemColumnWidth);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((layoutSettings.itemColumnWidth / 2.0f) - (layoutSettings.itemGroupWidth / 2.0f)));

						if (resource != nullptr)
						{
							ResourceType resType = static_cast<ResourceType>(resource->GetType());

							switch (resType)
							{
							case ResourceType::TEXTURE:
								DrawTextureItem(resIdx, (TextureRes&)*resource.get());
								break;
							case ResourceType::SCENE:
								DrawSceneItem(resIdx, (SceneRes&)*resource.get());
								break;
							case ResourceType::SCRIPT:
								DrawScriptItem(resIdx, (ScriptRes&)*resource.get());
								break;
							case ResourceType::SHADER:break;
							case ResourceType::NONE:
							default:
								break;
							}
						}

						ImGui::PopID();
						ImGui::EndGroup();

						resIdx++;
					}
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
			if (ImGui::MenuItem("Open with System"))
			{
				Utils::OpenFileWithSystem(textureResource.GetPath());
			};

			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) 
			{ 
				_DeleteAssetState = textureResource.GetPath();
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
					[&](void* data) {
						Utils::RemoveFile(_DeleteAssetState);
						_DeleteAssetState = nullptr;
					});
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
		/* there is a change that the image may not be loaded yet if currently in the background thread. in this case we want to draw a default texture */
		ImTextureID thumbnailTexture = 0;
		if (textureResource.GetTexture() == nullptr)
		{
			thumbnailTexture = (ImTextureID)(INT_PTR)_resourcesService->GetDefaultTexture()->GetTexture()->GetHandle();
		}
		else
		{
			thumbnailTexture = (ImTextureID)(INT_PTR)textureResource.GetTexture()->GetHandle();
		}

		ImGui::Image(thumbnailTexture, ImVec2(layoutSettings.itemGroupWidth, layoutSettings.itemGroupWidth));
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
			if (ImGui::MenuItem("Copy")) { WYRD_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { WYRD_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { WYRD_TRACE("Texture Paste"); };
			ImGui::Separator();

			if (ImGui::MenuItem("Properties"))
			{
				_EventService->Publish(Events::EventType::SelectedAssetChanged, std::make_unique<Events::SelectedAssetChangedArgs>(&sceneResource));
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) 
			{ 
				_dialogService->OpenConfirmDialog(_EditorLayer, "Are you sure want to delete?", 
				[&](void* data) {
					Utils::RemoveFile(_SelectedResource->GetPath());
					_SelectedResource = nullptr;
				});
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
			if (ImGui::MenuItem("Copy")) { WYRD_TRACE("Texture Copied"); };
			if (ImGui::MenuItem("Cut")) { WYRD_TRACE("Texture Cut"); };
			if (ImGui::MenuItem("Paste")) { WYRD_TRACE("Texture Paste"); };
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
}