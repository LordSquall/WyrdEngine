#pragma once

#include "osrpch.h"
#include "core/Log.h"

#include "Exporter.h"
#include "services/ServiceManager.h"
#include "services/WorkspaceService.h"
#include "support/Utils.h"

namespace Osiris::Editor
{
	Exporter::Exporter()
	{
		
	}

	Exporter::~Exporter() {}


	void Exporter::Export()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		std::ofstream core;
		core.open(Utils::GetBuildsFolder() + "/Core.dat", std::ios::out | std::ios::binary);
		
		core.close();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		/* game title */
		game.write(workspaceService->GetCurrentProject()->name.c_str(), 100);
		game.close();

		OSR_CORE_TRACE("Exporting Windows Game Files");
	}
}