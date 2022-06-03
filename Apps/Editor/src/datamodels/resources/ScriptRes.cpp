#include "wyrdpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"
#include "services/SimulationService.h"

#include "datamodels/logging/LogMessage.h"

namespace Wyrd::Editor
{
	ScriptRes::ScriptRes(const std::filesystem::path& filepath) : Resource(filepath.stem().string(), filepath)
	{

	}

	bool ScriptRes::Load()
	{
		return true;
	}
}
