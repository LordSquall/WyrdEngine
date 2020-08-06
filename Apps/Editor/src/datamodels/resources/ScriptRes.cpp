#pragma once

#include "osrpch.h"

#include "ScriptRes.h"

#include "services/ServiceManager.h"
#include "services/SimulationService.h"

#include "datamodels/logging/LogMessage.h"

namespace Osiris::Editor
{
	ScriptRes::ScriptRes(const std::string& path) : _path(path)
	{
		/* create a new scriped object template from the source code */
		Osiris::Behaviour::CreateCustomClassResult result = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->AddCustomScriptClass(Utils::GetFilename(path), _path);

		if (result.success == true)
		{
			Script.swap(result.customClass);
		}
		else
		{
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::AddLogEntry,
				std::make_shared<Events::AddLogEntryArgs>(Severity::Error, GetName() + " | " + result.message));
		}
	}

	void ScriptRes::Reload()
	{
		/* get the source code from the lua file */
		std::ifstream t(_path);
		std::string source = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		///* create a new scriped object template from the source code */
		//auto createResult = ServiceManager::Get<SimulationService>(ServiceManager::Simulation)->LoadScriptableObjectTemplate(ScriptedObjectTemplate, source);

		//if (createResult.result == true)
		//{
		//	ScriptedObjectTemplate = createResult.scriptedObjectTemplate;
		//}
		//else
		//{
		//	ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::AddLogEntry,
		//		std::make_shared<Events::AddLogEntryArgs>(Severity::Error, _name + " | " + createResult.error));
		//}
	}
}
