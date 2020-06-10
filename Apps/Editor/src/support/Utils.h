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

		/* Asset Folder Functions */
		static std::string GetAssetFolder();
		static std::string GetEditorResFolder();

		/* Dialog Functions */
		static std::string OpenFileDialog(const std::string& filter);
		static std::string OpenFolderDialog();
		static std::string SaveFileDialog(const std::string& name, const std::string& filter);
		static std::string SaveAsFileDialog(const std::string& filter);

		/* File Path Functions */
		static bool FileExists(const std::string& filename);
		static std::string GetPath(const std::string& filename);
		static std::string GetFilename(const std::string& path, bool addExtension = false);
		static std::string GetFileExtension(const std::string& path);
		static std::string GetRelativeAssetPath(const std::string& path);
		static void CreateFolder(const std::string& path);

		/* File Functions */
		static bool CreateRawFile(const std::string& filename, const std::string& content);

		static const std::string Utils::BuildPath(std::initializer_list<std::reference_wrapper<std::string>> args);

		static std::vector<std::string> GetFolderList(const std::string& directory, bool asFullPaths);
		static std::vector<std::string> Utils::GetFileList(const std::string& directory, bool asFullPaths = false, bool recursive = false);

		static void CreateProjectFileStructure(const std::string& rootFolder);

		/* System operations */
		static void OpenFileWithSystem(const std::string& file);

		/* String Convertors */
		static bool ToBool(std::string& value);
		static unsigned int ToUInt(std::string& value);
		static int ToInt(std::string& value);

	private:
		static void SwapSlashes(std::string& path, const std::string& find, const std::string& replace);
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
	};


}