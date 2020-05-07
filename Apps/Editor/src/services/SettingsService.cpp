#pragma once

#include "osrpch.h"
#include "SettingsService.h"

namespace Osiris::Editor
{
	
	void SettingsService::OnCreate()
	{
		_IniParser = std::make_shared<IniParser>("config.ini");
	}

	void SettingsService::OnDestroy()
	{
		_IniParser->Save("config.ini");
	}
}