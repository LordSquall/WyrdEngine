#pragma once

/* core osiris includes */
#include <core/export.h>

/* local includes */
#include "services/IService.h"
#include "events/EditorEvents.h"

/* external includes */
#include "support/IniParser.h"

/* Settings defines - Sections */
#define CONFIG_PROJECT						"Project"
#define CONFIG_PROJECT__DEFAULT				"default"

#define CONFIG_WINDOW						"Window"
#define CONFIG_WINDOW__WIDTH				"width"
#define CONFIG_WINDOW__HEIGHT				"height"
#define CONFIG_WINDOW__X					"x"
#define CONFIG_WINDOW__Y					"y"

#define CONFIG_RENDERDOC					"Preferences-RenderDoc"
#define CONFIG_RENDERDOC__ENABLED			"enabled"
#define CONFIG_RENDERDOC__DISPLAYOVERLAY	"display_overlay"
#define CONFIG_RENDERDOC__AUTOOPEN			"auto_open"
#define CONFIG_RENDERDOC__MAXFILEPATH		"max_filepath"
#define CONFIG_RENDERDOC__CAPTUREDIR		"capture_dir"


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