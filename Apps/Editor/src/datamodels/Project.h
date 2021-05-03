#pragma once

/* Core includes */
#include <core/UID.h>

#include <jsonxx.h>

namespace Wyrd::Editor {

	struct ExportSettings
	{
		std::map<UID, std::string> exportableScenes;

		UID initialScene;

		int width;
		int height;
	};

	/* Editor Project Data Model Structure */
	class Project
	{
	public:
		/* Constructors */
		Project();
		Project(std::string name);
		~Project() {}
		
		/**
		* @brief Get the current project export settings
		* @return Export settings
		*/
		ExportSettings& GetExportSettings();

		/**
		 * @brief Serialise the gameobject into a json object
		 * @return json object
		*/
		jsonxx::Object ToJson();

		/**
		 * @brief Deserialise a json object into a gameobject
		 * @param object json object
		 * @return true is successful
		*/
		bool FromJson(jsonxx::Object& object);

	public:
		std::string name;
		std::string initialScene;

	private:
		ExportSettings _ExportSettings;
	};
}