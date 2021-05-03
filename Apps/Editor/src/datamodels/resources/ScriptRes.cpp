#pragma once

#include "wyrdpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"
#include "services/SimulationService.h"

#include "datamodels/logging/LogMessage.h"

namespace Wyrd::Editor
{
	ScriptRes::ScriptRes(const std::string& path) : Resource(Utils::GetFilename(path), path)
	{

	}

	bool ScriptRes::Load()
	{
		return true;
	}
}
