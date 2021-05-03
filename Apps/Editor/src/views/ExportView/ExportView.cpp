#pragma once

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
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
		_SimulationService = ServiceManager::Get<SimulationService>(ServiceManager::Simulation);
	}

	ExportView::~ExportView()
	{
		WYRD_TRACE("ExportView::~");
	}
	
	void ExportView::OnEditorRender()
	{
		std::shared_ptr<Project> project = _WorkspaceService->GetCurrentProject();

		if (ImGui::Button("Export"))
		{
			_WorkspaceService->SaveScene();

			ExportManager::Export();

			auto p = std::filesystem::current_path();

			std::string windowsBinPath = _SettingsService->GetSetting(CONFIG_WINDOWSPLAYER, CONFIG_WINDOWSPLAYER__BINPATH, std::string("../../bin/Debug/Player/Player.exe"));

			ShellExecuteA(NULL, "open", windowsBinPath.c_str(), "", NULL, SW_SHOW);
		}

		if (ImGui::BeginListBox("Scenes"))
		{
			if (project != nullptr)
			{
				for (auto& sceneUID : project->GetExportSettings().exportableScenes)
				{
					std::string name = sceneUID.second;

					if (ImGui::Selectable(name.c_str(), sceneUID.first == _SelectedSceneUID))
					{
						_SelectedSceneUID = sceneUID.first;
					}
				}
			}
			ImGui::EndListBox();
		}
		
		ImGui::Text("Initial Scene: %s", project != nullptr ? project->GetExportSettings().initialScene.str().c_str() :  "NO PROJECT LOADED");

		//int size[2];
		ImGui::InputInt2("Size", (int*)&project->GetExportSettings().width);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SCENE"))
			{
				UID* sceneUID = (UID*)payload->Data;
				auto sceneResource = _ResourceService->GetResourceByID<SceneRes>(*sceneUID);
				project->GetExportSettings().exportableScenes[*sceneUID] = sceneResource->GetName();
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::TextButton("Set as Default Scene", _SelectedSceneUID != UID()))
		{
			project->GetExportSettings().initialScene = _SelectedSceneUID;
		}
	}
}