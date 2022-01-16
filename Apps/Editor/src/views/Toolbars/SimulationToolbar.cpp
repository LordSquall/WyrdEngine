#pragma once

#include "SimulationToolbar.h"

#include "support/ImGuiUtils.h"
#include "services/ServiceManager.h"

#include "core/Application.h"
#include "core/Layer.h"

#include <imgui.h>

namespace Wyrd::Editor
{
	SimulationToolbar::SimulationToolbar() : EditorToolbarBase()
	{
		auto _resourceService = ServiceManager::Get<ResourceService>();

		/* cache icon resources */
		_playButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_play");
		_stopButtonIcon = _resourceService->GetIconLibrary().GetIcon("common", "sim_stop");
	}

	void SimulationToolbar::OnToolbarRender()
	{
		bool isAvailable = ServiceManager::Get<SimulationService>()->IsAvailable();
		bool isRunning = ServiceManager::Get<SimulationService>()->IsRunning();
		
		if (ImGui::IconButton(_playButtonIcon, 4, !isRunning && isAvailable) == true)
		{
			ServiceManager::Get<SimulationService>()->Start();
		}
		ImGui::SameLine();
		if (ImGui::IconButton(_stopButtonIcon, 5, isRunning && isAvailable) == true)
		{
			ServiceManager::Get<SimulationService>()->Stop();
		}
	}
}