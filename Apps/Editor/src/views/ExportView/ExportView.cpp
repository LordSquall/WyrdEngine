/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>

/* local include */
#include "ExportView.h"
#include "export/ExportManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	ExportView::ExportView(EditorLayer* editorLayer) : EditorViewBase("Export View", editorLayer)
	{
		/* retrieve services */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();
		_EventService = ServiceManager::Get<EventService>();
		_ResourceService = ServiceManager::Get<ResourceService>();
		_SettingsService = ServiceManager::Get<SettingsService>();
		_SimulationService = ServiceManager::Get<SimulationService>();
	}

	ExportView::~ExportView()
	{
		WYRD_TRACE("ExportView::~");
	}
	
	void ExportView::OnEditorRender()
	{
		std::shared_ptr<Project> project = _WorkspaceService->GetCurrentProject();

		if (ImGui::TextButton("Export", true))
		{
			_WorkspaceService->SaveScene();

			if (ExportManager::Export())
			{
				auto p = std::filesystem::current_path();

				std::string windowsBinPath = _SettingsService->GetSetting(CONFIG_WINDOWSPLAYER, CONFIG_WINDOWSPLAYER__BINPATH, std::string("..\\..\\bin\\Debug\\Player\\Player.exe"));

				Utils::OpenFileWithSystem(windowsBinPath);
			}
		}

		if (ImGui::BeginListBox("Scenes"))
		{
			if (project != nullptr)
			{
				auto iter = project->GetExportSettings().exportableScenes.begin();
				std::map<Wyrd::UID, std::string>::iterator toRemoveIter;
				bool toRemove = false;

				while (iter != project->GetExportSettings().exportableScenes.end())
				{
					std::string name = iter->second;

					if (project->GetExportSettings().initialScene == iter->first)
					{
						name += " (default)";
					}

					if (ImGui::Selectable(name.c_str(), iter->first == _SelectedSceneUID))
					{
						_SelectedSceneUID = iter->first;
					}

					/* Drag and Drop */
					//if (ImGui::BeginDragDropSource())
					//{
					//	ImGui::SetDragDropPayload("DND_ENTITY", &e, sizeof(Entity));
					//	ImGui::EndDragDropSource();
					//}

					if (ImGui::IsItemHovered())
					{
						// Tooltip
					}

					if (ImGui::BeginPopupContextItem())
					{

						if (ImGui::MenuItem("Remove"))
						{
							toRemoveIter = iter;
							toRemove = true;
						}
						ImGui::EndPopup();
					}

					++iter;
				}

				if (toRemove == true)
				{
					project->GetExportSettings().exportableScenes.erase(toRemoveIter);
				}
			}
			ImGui::EndListBox();

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_SCENE))
				{
					UID* sceneUID = (UID*)payload->Data;
					auto sceneResource = _ResourceService->GetResourceByID<SceneRes>(*sceneUID);
					project->GetExportSettings().exportableScenes[*sceneUID] = sceneResource->GetName();

					// if this is the only scene in the list we want to make the initial scene
					if (project->GetExportSettings().exportableScenes.size() == 1)
					{
						project->GetExportSettings().initialScene = *sceneUID;
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		
		if (project != nullptr)
		{
			ImGui::InputInt2("Size", (int*)&project->GetExportSettings().width);
		}

		if (ImGui::TextButton("Set as Default Scene", _SelectedSceneUID != UID()))
		{
			project->GetExportSettings().initialScene = _SelectedSceneUID;
		}
	}
}