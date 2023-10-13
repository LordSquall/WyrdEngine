#pragma once

/* core wyrd includes */
#include <core/export.h>

/* local includes */
#include "services/ServiceTypes.h"
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

#define CONFIG_SCENEVIEWER								"Preferences-SceneViewer"
#define CONFIG_SCENEVIEWER__ENABLED						"enabled"
#define CONFIG_SCENEVIEWER__XSPACING					"x_spacing"
#define CONFIG_SCENEVIEWER__YSPACING					"y_spacing"
#define CONFIG_SCENEVIEWER__COLS						"columns"
#define CONFIG_SCENEVIEWER__ROWS						"rows"
#define CONFIG_SCENEVIEWER__BGBOLOR						"bgcolor"
#define CONFIG_SCENEVIEWER__CAM_PERSPECTIVE_NEAR		"cam_perspective_near"
#define CONFIG_SCENEVIEWER__CAM_PERSPECTIVE_FAR			"cam_perspective_far"
#define CONFIG_SCENEVIEWER__CAM_PERSPECTIVE_FOV			"cam_perspective_fov"
#define CONFIG_SCENEVIEWER__CAM_PERSPECTIVE_ASPECT		"cam_perspective_aspect"

#define CONFIG_RENDERDOC					"Preferences-RenderDoc"
#define CONFIG_RENDERDOC__ENABLED			"enabled"
#define CONFIG_RENDERDOC__DISPLAYOVERLAY	"display_overlay"
#define CONFIG_RENDERDOC__AUTOOPEN			"auto_open"
#define CONFIG_RENDERDOC__MAXFILEPATH		"max_filepath"
#define CONFIG_RENDERDOC__CAPTUREDIR		"capture_dir"

#define CONFIG_WINDOWSPLAYER				"Preferences-Windows-Player"
#define CONFIG_WINDOWSPLAYER__BINPATH		"bin_path"

#define CONFIG_EXTERNALTOOLS				"Preferences-External-Tools"
#define CONFIG_EXTERNALTOOLS_TEXTUREEDITOR	"texture_editor"

namespace Wyrd::Editor
{
	class SettingsService : public IService
	{
	public:
		SettingsService() {}
		~SettingsService() {}

		inline std::string GetSetting(std::string section, std::string name, std::string defaultValue) { return _IniParser->GetValue(section, name, defaultValue); }
		inline void SetSetting(std::string value, std::string section, std::string name) { _IniParser->SetValue(value, section, name); }

		static ServiceType GetTypeID() { return ServiceType::Settings; }
	private:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		std::shared_ptr<IniParser> _IniParser;
	};
}