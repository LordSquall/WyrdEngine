#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Layer.h>

namespace Wyrd::Editor
{
	enum class FileStatus { created, modified, erased, renamed };

	class FileWatcher
	{
	public:
		FileWatcher();
		void Initialise(const std::string& path, std::chrono::duration<int, std::milli> delay);

		void Start(const std::function<void(std::string, FileStatus)>& action);
		void End();

		std::string Path;
		std::chrono::duration<int, std::milli> Delay;
		std::unordered_map<std::string, std::filesystem::file_time_type> Paths;
		bool Running;
		std::thread Thread;
		std::function<void(std::string, FileStatus)> Action;
	};
}