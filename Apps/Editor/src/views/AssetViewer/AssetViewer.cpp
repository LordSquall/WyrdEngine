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
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"
#include "views/MaterialEditorView/MaterialEditorView.h"

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
		int itemColumnCnt = 8;
		int itemLabelShortCharLimit = 9;
	} layoutSettings;

	AssetViewer::AssetViewer(EditorLayer* editorLayer) : EditorViewBase("Asset Viewer", editorLayer), _SelectedDirectory(""), _SelectedResource(-1), _DeleteDirectoryState(""), _DeleteAssetState("")
	{
		/* cache the service(s) */
		_Resources = ServiceManager::Get<ResourceService>();
		_Workspace = ServiceManager::Get<WorkspaceService>();
		_Settings = ServiceManager::Get<SettingsService>();
		_Events = ServiceManager::Get<EventService>();
		_Dialogs = ServiceManager::Get<DialogService>();

		/* register from events */
		_Events->Subscribe(Events::EventType::ProjectLoaded, [this](Events::EventArgs& args)
			{
				_SelectedDirectory = _Workspace->GetAssetsDirectory();
			});
	}

	void AssetViewer::OnEditorRender()
	{
		float navigationPanelWidth = 150.0f;
		float itemGroupWidth = 64.0f;
		static ResourceType selectedType = ResourceType::NONE;

		if (_Workspace->GetCurrentProject() != nullptr)
		{
			/* capture the width of the child area to determine the column count */
			//layoutSettings.itemColumnCnt = (int)(ImGui::GetContentRegionAvail().x / layoutSettings.itemGroupSize);

			ImGui::Text("Search: ");
			ImGui::SameLine(); 

			ImGui::InputText("##label", &_SearchCriteria);
			ImGui::SameLine();
						
			if (ImGui::IconButton(_Resources->RetrieveIcon("common", "data_filteroff"), 0, _SearchCriteria != "", ImVec2(16.0f, 16.0f)))
			{
				_SearchCriteria = "";
			}
			ImGui::SameLine();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			ImGui::SameLine();
			
			ImVec2 buttonSize = ImVec2(16.0f, 16.0f);
			bool parentDirAvailable = _SelectedDirectory != _Workspace->GetAssetsDirectory();
			if (ImGui::IconButton(_Resources->RetrieveIcon("common", "nav_up"), 2, parentDirAvailable, buttonSize))
			{
				_SelectedDirectory = _SelectedDirectory.parent_path();
				_SelectedResource = -1;
			}
			ImGui::SameLine();
			
			if (ImGui::IconButton(_Resources->RetrieveIcon("common", "nav_open"), 3, true, buttonSize))
			{
				Utils::OpenFolderInExplorer(_SelectedDirectory.string());
			}

			//ImGui::SliderFloat("Size", &layoutSettings.itemGroupSize, 16.0f, 512.0f);

			ImGui::BeginChildFrame(2, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

			/* Context Menu - No Selection */
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Import from file"))
				{
					std::optional<std::string> file = Utils::OpenFile({ { "All Files (*)", "*.*" } });
					if (file)
					{
						/* copy the file into the project */
						Utils::CopySingleFile(file.value(), _SelectedDirectory.string());
					}
				}
				if (ImGui::BeginMenu("Create..."))
				{
					if (ImGui::MenuItem("Scene"))
					{
						_Dialogs->OpenSingleEntryDialog(_EditorLayer, "Create new Scene", "Name", [&](std::string d) -> void {

							/* load the scene template */
							std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Scene.scene");

							/* populate the template tags */
							std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<SCENE_NAME>>", d.c_str());

							/* create the file */
							Utils::CreateRawFile(_SelectedDirectory / (d + ".scene"), populatedTemplateContent);

							WYRD_TRACE("Add Scene... {0}", d.c_str());
							});
					}
					ImGui::Separator();
					if (ImGui::MenuItem("ShaderStage")) 
					{
						_Dialogs->OpenCreateShaderStageDialog(_EditorLayer, "Create new ShaderStage", "Name", [&](std::string d, int t) -> void {

							/* determine extension from type */
							std::string ext = (t == 0 ? ".vs" : ".fs");

							/* load the Shader Stage template */
							std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\ShaderStage" + ext);
							
							/* populate the template tags */
							std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<SHADERSTAGE_NAME>>", d.c_str());

							/* create the file */
							Utils::CreateRawFile(_SelectedDirectory / (d + ext), populatedTemplateContent);

							WYRD_TRACE("Add Shader Stage... {0}", d.c_str());
							});
					}
					if (ImGui::MenuItem("Shader"))
					{
						_Dialogs->OpenSingleEntryDialog(_EditorLayer, "Create new Shader", "Name", [&](std::string d) -> void {

							/* load the material template */
							std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Shader.shader");

							/* populate the template tags */
							std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<SHADER_NAME>>", d.c_str());

							/* create the file */
							Utils::CreateRawFile(_SelectedDirectory / (d + ".shader"), populatedTemplateContent);

							WYRD_TRACE("Add Shader... {0}", d.c_str());
							});
					}
					if (ImGui::MenuItem("Material"))
					{
						_Dialogs->OpenSingleEntryDialog(_EditorLayer, "Create new Material", "Name", [&](std::string d) -> void {

							/* load the material template */
							std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Material.material");

							/* populate the template tags */
							std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<MATERIAL_NAME>>", d.c_str());

							/* create the file */
							Utils::CreateRawFile(_SelectedDirectory / (d + ".material"), populatedTemplateContent);

							WYRD_TRACE("Add Material... {0}", d.c_str());
						});
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Script"))
					{
						_Dialogs->OpenSingleEntryDialog(_EditorLayer, "Create new Script", "Name", [&](std::string d) -> void {

							/* load the material template */
							std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\Script.cs");

							/* populate the template tags */
							std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<CLASS_NAME>>", d.c_str());

							/* create the file */
							Utils::CreateRawFile(_SelectedDirectory / (d + ".cs"), populatedTemplateContent);

							WYRD_TRACE("Add Script... {0}", d.c_str());
						});
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::Selectable("Add Folder"))
				{
					Utils::CreateFolder((_SelectedDirectory / "New Folder").string());
				}
				ImGui::EndPopup();
			}

			uint32_t resIdx = 0;

			if (ImGui::BeginTable("assetItemsTable", layoutSettings.itemColumnCnt))
			{
				uint32_t resIdx = 0;
				
				std::vector<std::filesystem::path> directories;
				std::vector<std::filesystem::path> files;

				if (_SearchCriteria == "")
				{
					for (const auto& entry : std::filesystem::directory_iterator(_SelectedDirectory))
					{
						if (!std::filesystem::is_regular_file(entry.path().string()))
						{
							directories.push_back(entry);
						}
					}

					for (const auto& entry : std::filesystem::directory_iterator(_SelectedDirectory))
					{
						/* check if the file extension shouldn't be ignored */
						if (!_Resources->CheckIgnored(entry.path()))
						{
							if (std::filesystem::is_regular_file(entry.path().string()))
							{
								files.push_back(entry);
							}
						}
					}
				}
				else
				{
					std::vector<std::filesystem::path> paths;
					if (std::filesystem::exists(_SelectedDirectory) && std::filesystem::is_directory(_SelectedDirectory))
					{
						std::filesystem::recursive_directory_iterator dirpos{ _SelectedDirectory };

						std::copy_if(begin(dirpos), end(dirpos), std::back_inserter(paths),
							[](const std::filesystem::directory_entry& entry) {
								return entry.is_regular_file();
							}
						);
					}
					std::sort(paths.begin(), paths.end());

					for (auto& entry : paths)
					{
						const auto filename = entry.filename().string();
						if (filename.find(_SearchCriteria) != std::string::npos)
						{
							files.push_back(entry.string());
						}
					}
				}

				DrawItemTable(directories, files);

				ImGui::EndTable();
			}

			ImGui::EndChildFrame();
		}
	}

	void AssetViewer::OnEvent(Event& event)
	{
	}


	void AssetViewer::DrawItemTable(const std::vector<std::filesystem::path>& directories, const std::vector<std::filesystem::path>& files)
	{
		int resIdx = 0;

		for (auto& directory : directories)
		{
			uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;

			if (tableCursor == 0)
			{
				ImGui::TableNextRow();
			}

			ImGui::TableSetColumnIndex(tableCursor);

			ImVec2 cursor = ImGui::GetCursorPos();
			ImGui::PushID(resIdx);
			ImGui::Selectable("##title", _SelectedResource == resIdx, ImGuiSelectableFlags_None, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize));
			ImGui::PopID();
			ImGui::SetCursorPos(cursor);

			DrawFolderItem(resIdx, directory.c_str());

			resIdx++;
		}

		for (auto& file : files)
		{
			/* retrieve matching resource cache entry */
			std::shared_ptr<Resource> resource = _Resources->GetResourceByFilePath(file);

			uint32_t tableCursor = resIdx % layoutSettings.itemColumnCnt;
			
			if (tableCursor == 0)
			{
				ImGui::TableNextRow();
			}

			ImGui::TableSetColumnIndex(tableCursor);

			ImVec2 cursor = ImGui::GetCursorPos();
			ImGui::PushID(resIdx);
			if (ImGui::Selectable("##title", _SelectedResource == resIdx, ImGuiSelectableFlags_None, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize)))
			{
				_Events->Publish(Events::EventType::SelectedAssetChanged, std::make_unique<Events::SelectedAssetChangedArgs>(resource->GetResourceID()));
			}
			ImGui::PopID();
			ImGui::SetCursorPos(cursor);

			if (resource != nullptr)
			{
				ResourceType resType = static_cast<ResourceType>(resource->GetType());

				const Icon& thumbnailIcon = resource->GetThumbnail();

				/* Drag and Drop */
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					const UID resourceID = resource->GetResourceID();
					ImGui::SetDragDropPayload(resource->GetTypeTag().c_str(), &resourceID, sizeof(UID));
					ImGui::Image(thumbnailIcon, ImVec2(32, 32));
					ImGui::EndDragDropSource();
				}

				/* Tool Tip */
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(resource->GetName().c_str());
					ImGui::Text("Filename: %s", resource->GetPath().c_str());
					ImGui::Text("UID: %s", resource->GetResourceID().str().c_str());
					ImGui::EndTooltip();
				}

				/* Context Menu */
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
					}
					ImGui::EndPopup();
				}

				/* Input */
				if (ImGui::IsItemHovered())
				{
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						_SelectedResource = resIdx;

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						Utils::OpenFileWithSystem(resource->GetPath());
				}

				/* Visuals */
				ImGui::Image(thumbnailIcon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize));
				ImGui::TextClipped(resource->GetName().c_str(), layoutSettings.itemGroupSize, ImVec4(1, 1, 1, 1), "..");
			}
			else
			{
				DrawUnknownItem(resIdx, file.stem().string());
			}

			resIdx++;
		}
	}

	void AssetViewer::DrawFolderItem(int resID, const std::filesystem::path& directoryEntry)
	{
		std::filesystem::path p(directoryEntry);

		/* Drag and Drop */
	
		/* Tool Tip */
		if (ImGui::IsItemHovered())
		{
			/* No tool tip on folders */
		}

		/* Context Menu */
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				if (!Utils::DeleteFolder(directoryEntry))
				{
					WYRD_ERROR("Unable to delete folder at {0}", directoryEntry);
				}
			}
			ImGui::EndPopup();
		}

		/* Input */
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				_SelectedResource = resID;

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				_SelectedDirectory /= p.stem();
				_SelectedResource = -1;
			}
		}

		/* Visuals */
		ImGui::Image(_Resources->RetrieveIcon("common", "assets_folder"), ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(p.stem().string().c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "..");
	}

	void AssetViewer::DrawUnknownItem(int resID, const std::string& unknownResourceName)
	{
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
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				Utils::RemoveFile(unknownResourceName);
			}
			ImGui::EndPopup();
		}
		
		/* Input */
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				_SelectedResource = resID;
		}
		
		/* Visuals */
		ImVec2 rootPos = ImGui::GetCursorPos();

		ImGui::Image(_Resources->RetrieveIcon("common", "assets_unknown"), ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));
		ImGui::TextClipped(unknownResourceName.c_str(), layoutSettings.itemGroupSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "...");
	}

	void AssetViewer::DrawStatusIcon(const Icon& icon, const ImVec2& rootPos)
	{
		ImVec2 finalPos = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(rootPos.x + layoutSettings.itemGroupSize * 0.75);
		ImGui::SetCursorPosY(rootPos.y + layoutSettings.itemGroupSize * 0.75);
		ImGui::Image(icon, ImVec2(layoutSettings.itemGroupSize, layoutSettings.itemGroupSize) * 0.25, ImVec4(0.9, 0.1, 0.1, 1), ImVec4(1, 0, 0, 0));
		ImGui::SetCursorPos(finalPos);
	}
}