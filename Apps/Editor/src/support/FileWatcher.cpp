#pragma once

/* core includes */
#include <core/Log.h>

/* local includes */
#include "support/FileWatcher.h"

namespace Wyrd::Editor
{
	FileWatcher::FileWatcher() : Path(""), Delay(), Running(false)
	{

	}

	void FileWatcher::Initialise(const std::string& path, std::chrono::duration<int, std::milli> delay)
	{
		Path = path;
		Delay = delay;

		for (auto& file : std::filesystem::recursive_directory_iterator(path)) {

			Paths[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

	void FileWatcher::Start(const std::function<void(std::string, FileStatus)>& action)
	{
		WYRD_CORE_INFO("Starting File Watcher on {0}", Path);
		Action = action;

		Thread = std::thread{
			[&]()
			{
				std::map<std::string, std::string> deletedFiles;

				while (Running)
				{
					std::this_thread::sleep_for(Delay);

					auto it = Paths.begin();
					while (it != Paths.end())
					{
						if (!std::filesystem::exists(it->first))
						{
							deletedFiles.insert({ it->first, "HASH" });
						}
						it++;
					}

					for (auto& file : std::filesystem::recursive_directory_iterator(Path))
					{
						auto currentFileWriteTime = std::filesystem::last_write_time(file);

						auto el = Paths.find(file.path().string());
						bool found = el != Paths.end();
						if (!found)
						{
							Paths[file.path().string()] = currentFileWriteTime;
							Action(file.path().string(), FileStatus::created);
						}
						else
						{
							if (Paths[file.path().string()] != currentFileWriteTime)
							{
								Paths[file.path().string()] = currentFileWriteTime;
								Action(file.path().string(), FileStatus::modified);
							}
						}
					}
					

					auto deletedFilesIt = deletedFiles.begin();
					while (deletedFilesIt != deletedFiles.end())
					{
						if (!std::filesystem::exists(deletedFilesIt->first))
						{
							Paths.erase(deletedFilesIt->first);
							Action(deletedFilesIt->first, FileStatus::erased);
							deletedFilesIt = deletedFiles.erase(deletedFilesIt);
							
						}
						else
						{
							deletedFilesIt++;
						}
					}
				}
				WYRD_CORE_INFO("Ending File Watcher on {0}", Path);
			}
		};

		Running = true;
	}

	void FileWatcher::End()
	{
		if (Running == true)
		{
			Running = false;
			Thread.join();
		}
	}
}