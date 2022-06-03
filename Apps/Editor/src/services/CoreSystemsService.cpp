/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "CoreSystemsService.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	void CoreSystemsService::OnCreate()
	{
		WYRD_INFO("CoreSystemService::OnCreate()");
	}

	void CoreSystemsService::OnDestroy()
	{
		WYRD_INFO("CoreSystemService::OnDestroy()");
	}

	void CoreSystemsService::RegisterSystem(const std::string& name, ISystem* system)
	{
		_Systems[name] = system;

		WYRD_INFO("Registered System: {0}", name);
	}
}