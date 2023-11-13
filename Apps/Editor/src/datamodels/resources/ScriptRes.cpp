#include "wyrdpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"
#include "services/SimulationService.h"

#include "datamodels/logging/LogMessage.h"

namespace Wyrd::Editor
{
	ScriptRes::ScriptRes(const std::string& name, const UID& uid) : Resource(name, uid), PendingReload(false)
	{

	}

	Resource::IOResult ScriptRes::Load(const std::string& filepath)
	{
		return NotImplemented;
	}

	Resource::IOResult ScriptRes::Load(const jsonxx::Object& obj)
	{
		return NotImplemented;
	}

	Resource::IOResult ScriptRes::Save(const std::string& filepath) 
	{
		return NotImplemented;
	}
}
