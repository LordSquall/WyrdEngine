#pragma once

#include "PrimaryLayer.h"

#include <fstream>

bool PrimaryLayer::OnAttach()
{
	/* open the editor core data file */
	std::ifstream core("./Core.dat");
	if (core.good() == false)
	{
		OSR_CORE_ERROR("Unable to find Core.dat file");
		return false;
	}
	
	std::ifstream game("./Game.dat");
	if (game.good() == false)
	{
		OSR_CORE_ERROR("Unable to find Game.dat file");
		return false;
	}

	return true;
}

void PrimaryLayer::OnDetach() { }

void PrimaryLayer::OnUpdate(Timestep ts) { }

void PrimaryLayer::OnRender(Timestep ts, Renderer& renderer) { }