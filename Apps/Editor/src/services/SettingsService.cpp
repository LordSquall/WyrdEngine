/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "SettingsService.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	void SettingsService::OnCreate()
	{
		_ConfigIniParser = std::make_shared<IniParser>("config.ini");
		_PreferencesIniParser = std::make_shared<IniParser>("preferences.ini");

		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::SettingsUpdated, [this](Events::EventArgs& args)
			{
				_ConfigIniParser->Save("config.ini");
			});

		ServiceManager::Get<EventService>()->Subscribe(Events::EventType::SettingsUpdated, [this](Events::EventArgs& args)
			{
				_PreferencesIniParser->Save("preferences.ini");
			});
	}

	void SettingsService::OnDestroy()
	{
		_ConfigIniParser->Save("config.ini");
		_PreferencesIniParser->Save("preferences.ini");
	}
}