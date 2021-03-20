#pragma once

#include "PrimaryLayer.h"
#include "core/Log.h"

#include <fstream>

bool PrimaryLayer::OnAttach()
{
	return true;
}

void PrimaryLayer::OnDetach() { }

void PrimaryLayer::OnUpdate(Timestep ts) { }

void PrimaryLayer::OnRender(Timestep ts, Renderer& renderer) { }