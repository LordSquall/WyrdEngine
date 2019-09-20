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

		static void SetRootProjectFolder(const std::string& rootFolder);

		static std::string GetAssetFolder();
		static std::string OpenFileDialog(const std::string& filter);
		static std::string OpenFolderDialog();
		static std::string SaveFileDialog(const std::string& filter);
		static std::string SaveAsFileDialog(const std::string& filter);

		static std::string GetPath(const std::string& filename);

		static void CreateProjectFileStructure(const std::string& rootFolder);

	private:
		static void SwapSlashes(std::string& path, const std::string& find, const std::string& replace);
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
	};


}