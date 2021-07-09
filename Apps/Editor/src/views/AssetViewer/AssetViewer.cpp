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
		float itemGroupSize = 64.0f;
		int itemColumnCnt = 4;
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
		_FolderIcon = _resourcesService->GetIconLibrary().GetIcon("common", "folder");
		_SceneIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_scene");
		_TextureIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_texture");
		_ScriptIcon = _resourcesService->GetIconLibrary().GetIcon("common", "assets_script");

		/* register from events */
		_EventService->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args)
			{
				_SelectedDirectory = _workspaceService->GetAssetsDirectory();
			});
	}

	void AssetViewer::OnEditorRender()
	{
		float navigationPanelWidth = 150.0f;
		float itemGroupWidth = 64.0f;
		static ResourceType selectedType = ResourceType::NONE;

		if (_workspaceService->GetCurrentProject() != nullptr)
		{
			/* capture the width of the child area to determine the column count */
			layoutSettings.itemColumnCnt = (int)(ImGui::GetContentRegionAvail().x / layoutSettings.itemGroupSize);

			ImGui::Text("Search: ");
			ImGui::SameLine(); 

			ImGui::InputText("##label", &_SearchCriteria);
			ImGui::SameLine();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::SameLine();
			
			if (ImGui::IconButton(_UnknownIcon, 0, true, ImVec2( 16.0f, 16.0f )))
			{
				_SearchCriteria = "";
			}
			ImGui::SameLine();
			
			if (ImGui::IconButton(_UnknownIcon, 2, _SelectedDirectory != _workspaceService->GetAssetsDirectory(), ImVec2(16.0f, 16.0f)))
			{
				_SelectedDirectory = _SelectedDirectory.parent_path();
			}
			ImGui::SameLine();
			
			if (ImGui::IconButton(_UnknownIcon, 3, true, ImVec2(16.0f, 16.0f)))
			{
				ShellExecuteA(NULL, "open", _SelectedDirectory.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
			}

			//ImGui::SliderFloat("Size", &layoutSettings.itemGroupSize, 16.0f, 512.0f);

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			uint32_t resIdx = 0;

			if (ImGui::BeginTable("assetItemsTable", layoutSettings.itemColumnCnt))
			{
				uint32_t resIdx = 0;

				for (const auto& entry : std::filesystem::directory_iterator(_SelectedDirectory))
				{
					if (!std::filesystem::is_regular_file(entry.path().string()))
					{
						uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;

						if (tableCursor == 0)
						{
							ImGui::TableNextRow();
						}

						ImGui::TableSetColumnIndex(tableCursor);

						bool selected = false;

						ImVec2 cursor = ImGui::GetCursorPos();
						ImGui::PushID(resIdx);
						ImGui::Selectable("##title", selected, ImGuiSelectableFlags_None, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize));
						ImGui::PopID();
						ImGui::SetCursorPos(cursor);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							WYRD_TRACE("Folder Double Clicked");
							_SelectedDirectory /= entry.path().stem();
						}

						ImGui::Image(*_FolderIcon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

						ImGui::TextClipped(entry.path().stem().string().c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "..");

						resIdx++;
					}
				}

				for (const auto& entry : std::filesystem::directory_iterator(_SelectedDirectory))
				{
					if (std::filesystem::is_regular_file(entry.path().string()))
					{
						uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;

						if (tableCursor == 0)
						{
							ImGui::TableNextRow();
						}

						ImGui::TableSetColumnIndex(tableCursor);

						bool selected = false;

						ImVec2 cursor = ImGui::GetCursorPos();
						ImGui::PushID(resIdx);
						ImGui::Selectable("##title", selected, ImGuiSelectableFlags_None, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize));
						ImGui::PopID();
						ImGui::SetCursorPos(cursor);

						/* retrieve matching resource cache entry */
						auto& resource = _resourcesService->GetResourceByFilePath(entry.path().string());

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
							case ResourceType::SHADER:
							case ResourceType::NONE:
							default:
								break;
							}
						}
						else
						{
							DrawUnknownItem(resIdx, entry.path().string());
						}

						resIdx++;
					}
				}

				ImGui::EndTable();
			}

			ImGui::EndChildFrame();
		}
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
			
		}
		return true;
	}

	//void AssetViewer::DrawAssetsItems()
	//{
	//	uint32_t resIdx = 0;
	//
	//	if (ImGui::BeginTable("assetItemsTable", layoutSettings.itemColumnCnt, 0))
	//	{
	//		uint32_t resIdx = 0;
	//
	//		//	if (!std::filesystem::is_regular_file(entry.path().string()))
	//		//	{
	//		//		ImGui::BeginGroup();
	//
	//		//		ImGui::PushID(entry.path().string().c_str());
	//
	//		//		ImGui::Button(entry.path().string().c_str());
	//
	//		//		ImGui::PopID();
	//
	//		//		ImGui::EndGroup();
	//		//	}
	//		//	else
	//		//	{
	//		//		/* retrieve matching resource cache entry */
	//		//		auto& resource = _resourcesService->GetResourceByFilePath(entry.path().string());
	//
	//		//		if (resource == nullptr)
	//		//		{
	//		//			WYRD_CORE_TRACE("Resource Not Found: {0}", entry.path().string());
	//		//		}
	//
	//		//		/* create file name */
	//		//		std::string fullName = entry.path().string();
	//		//		auto lastSlash = fullName.find_last_of("/\\");
	//		//		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	//		//		std::string name = fullName.substr(lastSlash, fullName.size() - lastSlash);
	//
	//
	//		//		ImGui::BeginGroup();
	//
	//		//		ImGui::PushID(entry.path().string().c_str());
	//
	//		//		/* caculate the additional text size */
	//		//		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
	//
	//		//		// DEBUG ONLY
	//		//		//ImGuiWindow* window = ImGui::GetCurrentWindowRead();
	//		//		//const ImVec2 p0 = window->DC.CursorPos;
	//		//		//ImGui::GetWindowDrawList()->AddRect(p0, p0 + ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemColumnHeight), ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));
	//
	//
	//		//		/* selectable interface */
	//		//		bool selected = _SelectedResource == resource.get();
	//		//		if (ImGui::Selectable("##title", selected, ImGuiSelectableFlags_SelectOnClick, ImVec2(layoutSettings.itemColumnWidth, layoutSettings.itemGroupWidth + textSize.y)))
	//		//		{
	//		//			_SelectedResource = resource.get();
	//		//		}
	//
	//		//		ImGui::SameLine();
	//		//		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - layoutSettings.itemColumnWidth);
	//		//		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((layoutSettings.itemColumnWidth / 2.0f) - (layoutSettings.itemGroupWidth / 2.0f)));
	//
	//		//		if (resource != nullptr)
	//		//		{
	//		//			ResourceType resType = static_cast<ResourceType>(resource->GetType());
	//
	//		//			switch (resType)
	//		//			{
	//		//			case ResourceType::TEXTURE:
	//		//				DrawTextureItem(resIdx, (TextureRes&)*resource.get());
	//		//				break;
	//		//			case ResourceType::SCENE:
	//		//				DrawSceneItem(resIdx, (SceneRes&)*resource.get());
	//		//				break;
	//		//			case ResourceType::SCRIPT:
	//		//				DrawScriptItem(resIdx, (ScriptRes&)*resource.get());
	//		//				break;
	//		//			case ResourceType::SHADER:break;
	//		//			case ResourceType::NONE:
	//		//			default:
	//		//				break;
	//		//			}
	//		//		}
	//
	//		//		ImGui::PopID();
	//		//		ImGui::EndGroup();
	//
	//		//		resIdx++;
	//		//	}
	//		//}
	//			
	//		ImGui::EndTable();
	//	}
	//}

	void AssetViewer::DrawTextureItem(int resID, TextureRes& textureResource)
	{
		/* there is a change that the image may not be loaded yet if currently in the background thread. in this case we want to draw a default texture */
		ImTextureID thumbnailTexture = 0;
		if (textureResource.GetTexture() == nullptr)
			thumbnailTexture = (ImTextureID)(INT_PTR)_resourcesService->GetDefaultTexture()->GetTexture()->GetHandle();
		else
			thumbnailTexture = (ImTextureID)(INT_PTR)textureResource.GetTexture()->GetHandle();

		/* Drag and Drop */
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
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

		/* Context Menu */
		if (ImGui::BeginPopupContextItem())
		{
			ImGui::MenuItem(textureResource.GetName().c_str());
			if (ImGui::MenuItem("Open with System"))
			{
				Utils::OpenFileWithSystem(textureResource.GetPath());
			}
			ImGui::EndPopup();
		}

		/* Input */
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				WYRD_TRACE("Texture Clicked");
		}

		/* Visuals */
		ImGui::Image(thumbnailTexture, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec2(0,0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(textureResource.GetName().c_str(), layoutSettings.itemGroupSize, ImVec4(1, 1, 1, 1), "..");
	}

	void AssetViewer::DrawSceneItem(int resID, SceneRes& sceneResource)
	{
		/* Drag and Drop */
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
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

		/* Context Menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Properties"))
			{
				_EventService->Publish(Events::EventType::SelectedAssetChanged, std::make_unique<Events::SelectedAssetChangedArgs>(&sceneResource));
			}
			ImGui::EndPopup();
		}

		/* Input */
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->LoadScene(sceneResource.GetPath());
		}

		/* Visuals */
		ImGui::Image(*_SceneIcon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(sceneResource.GetName().c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");

	}

	void AssetViewer::DrawScriptItem(int resID, ScriptRes& scriptResource)
	{
		/* Drag and Drop */
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
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

		/* Context Menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Properties"))
			{
				_EventService->Publish(Events::EventType::SelectedAssetChanged, std::make_unique<Events::SelectedAssetChangedArgs>(&scriptResource));
			}
			ImGui::EndPopup();
		}

		/* Input */
		if (ImGui::IsItemHovered())
		{
			// TODO
		}

		/* Visuals */
		ImGui::Image(*_ScriptIcon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(scriptResource.GetName().c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");
	}

	void AssetViewer::DrawUnknownItem(int resID, std::string& unknownResourceName)
	{
		/* unknown items don's have any model, so we just want to extract the file name */
		std::string filename = Utils::GetFilename(unknownResourceName, true);

		/* Drag and Drop */
		// Not supported on unknown items

		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Filename: %s", unknownResourceName.c_str());
			ImGui::EndTooltip();
		}

		/* Context Menu */
		// Not supported on unknown items

		/* Input */
		if (ImGui::IsItemHovered())
		{
			// TODO
		}

		Utils::GetFilename(unknownResourceName, true);

		/* Visuals */
		ImGui::Image(*_UnknownIcon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(filename.c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");
	}
}