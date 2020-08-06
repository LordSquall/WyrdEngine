#pragma once

#include "osrpch.h"
#include "core/Log.h"

#include "Exporter.h"

#include "support/Utils.h"

namespace Osiris::Editor
{
	Exporter::Exporter()
	{
		
	}

	Exporter::~Exporter() {}


	void Exporter::Export()
	{
		std::ofstream core;
		core.open(Utils::GetBuildsFolder() + "/Core.dat", std::ios::out | std::ios::binary);
		
		core.close();

		std::ofstream game;
		game.open(Utils::GetBuildsFolder() + "/Game.dat", std::ios::out | std::ios::binary);

		/* game title */
		game.write("Hello World", 100);
		game.close();

		OSR_CORE_TRACE("Exporting Windows Game Files");
	}
}