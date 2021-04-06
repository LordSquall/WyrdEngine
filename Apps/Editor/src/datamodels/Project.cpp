#pragma once
#include "osrpch.h"

/* Local includes */
#include "Project.h"

namespace Osiris::Editor
{
	Project::Project() : name("Untitled"), initialScene("initialScene") { }

	Project::Project(std::string name) : name(name), initialScene("initialScene") { }

	ExportSettings& Project::GetExportSettings()
	{
		return _ExportSettings;
	}

	jsonxx::Object Project::ToJson()
	{
		jsonxx::Object object;

		/* name */
		object << "name" << name;

		/* initial scene */
		object << "initialScene" << initialScene;

		/* export settings */
		jsonxx::Object exportSettingsObj;

		jsonxx::Array exportableScenes;
		for (auto& scene : _ExportSettings.exportableScenes)
		{
			jsonxx::Object sceneObj;
			sceneObj << "uid" << scene.first.str();
			sceneObj << "name" << scene.second;
			exportableScenes << sceneObj;
		}

		exportSettingsObj << "initialScene" << _ExportSettings.initialScene.str();

		exportSettingsObj << "exportableScenes" << exportableScenes;

		object << "exportSettings" << exportSettingsObj;

		return object;
	}

	bool Project::FromJson(jsonxx::Object& object)
	{
		/* name */
		name = object.get<jsonxx::String>("name", "default");

		/* initial scene */
		initialScene = object.get<jsonxx::String>("initialScene", "default");

		/* export settings */
		if (object.has<jsonxx::Object>("exportSettings"))
		{
			jsonxx::Object exportSettings = object.get<jsonxx::Object>("exportSettings");

			/* export scenes  */
			if (exportSettings.has<jsonxx::Array>("exportableScenes"))
			{
				jsonxx::Array exportableScenes = exportSettings.get<jsonxx::Array>("exportableScenes");

				for (size_t i = 0; i < exportableScenes.size(); i++)
				{
					jsonxx::Object sceneJson = exportableScenes.get<jsonxx::Object>((int)i);

					_ExportSettings.exportableScenes[UID(sceneJson.get<jsonxx::String>("uid"))] = sceneJson.get<jsonxx::String>("name");
				}
			}

			/* initial scene */
			if (exportSettings.has<jsonxx::String>("initialScene"))
			{
				_ExportSettings.initialScene = UID(exportSettings.get<jsonxx::String>("initialScene"));
			}
			else
			{
				_ExportSettings.initialScene = UID();
			}
		}

		return true;
	}
}

