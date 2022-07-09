#pragma once

/* Core includes */
#include <jsonxx.h>

namespace Wyrd::Editor {

	struct ProjectEntry
	{
		std::string name;
		std::string path;
	};

	/* Editor Project Data Model Structure */
	class RecentProjects
	{
	public:
		/* Constructors */
		RecentProjects();
		~RecentProjects() {}

		/**
		* @brief Populate the internal map of recent project from a file
		*/
		void Populate();

		/**
		* @brief Add a new project entry to the internal list
		*/
		bool AddProject(const std::string& name, const std::string& path);

		/**
		* @brief Remove a project entry from the internal list
		*/
		bool RemoveProject(const std::string& path);

		/**
		* @brief Save the interal list to disk
		* @return true is file was succesfully saved
		*/
		bool Save();

		/**
		* @brief Retrieve a read only reference to the internally loaded project map
		*/
		inline const std::map<std::string, ProjectEntry>& GetMap() const { return _RecentProjectMap; };

	private:
		std::map<std::string, ProjectEntry> _RecentProjectMap;
	};
}