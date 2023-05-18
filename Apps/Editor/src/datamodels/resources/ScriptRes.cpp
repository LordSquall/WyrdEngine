#include "wyrdpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"
#include "services/SimulationService.h"

#include "datamodels/logging/LogMessage.h"

namespace Wyrd::Editor
{
	ScriptRes::ScriptRes(const std::filesystem::path& filepath, const UID& uid) : Resource(filepath.stem().string(), filepath, uid), PendingReload(false)
	{

	}

	int ScriptRes::Load()
	{
		return 0;
	}
}
