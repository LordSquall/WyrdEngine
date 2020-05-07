#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "events/EditorEvents.h"

#include "support/IniParser.h"

namespace Osiris::Editor
{
	class SettingsService : public IService
	{
	public:
		SettingsService() {}
		~SettingsService() {}

		inline std::string GetSetting(std::string section, std::string name, std::string defaultValue) { return _IniParser->GetValue(section, name, defaultValue); }
		inline void SetSetting(std::string value, std::string section, std::string name) { _IniParser->SetValue(value, section, name); }

	private:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		std::shared_ptr<IniParser> _IniParser;
	};
}