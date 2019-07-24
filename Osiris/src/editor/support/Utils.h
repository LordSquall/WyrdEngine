#pragma once
#include "core/export.h"
#include "osrpch.h"

namespace Osiris::Editor
{
	class Utils 
	{
	public:
		Utils();
		~Utils();

		std::string GetAssetFolder();
		std::string OpenFileDialog(const std::string& filter);
		std::string OpenFolderDialog();
		std::string SaveFileDialog(const std::string& filter);
		std::string SaveAsFileDialog(const std::string& filter);

		static std::string GetPath(const std::string& filename)
		{
			std::string directory;
			const size_t last_slash_idx = filename.rfind('/');
			if (std::string::npos != last_slash_idx)
			{
				directory = filename.substr(0, last_slash_idx);
			}
			return directory;
		}

	private:
		void SwapSlashes(std::string& path, const std::string& find,
			const std::string& replace);
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
	};


}