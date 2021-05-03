#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "SettingsService.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	
	void SettingsService::OnCreate()
	{
		_IniParser = std::make_shared<IniParser>("config.ini");


		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SettingsUpdated, [this](Events::EventArgs& args)
			{
				_IniParser->Save("config.ini");
			});
	}

	void SettingsService::OnDestroy()
	{
		_IniParser->Save("config.ini");
	}
}