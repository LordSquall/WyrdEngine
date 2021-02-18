#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/Structures.h>

/* local includes */


/* external includes */


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
		static std::string GetBuildsFolder();
		static std::string GetEditorResFolder();
		static std::string GetCacheFolder();
		static std::string GetTempFolder();

		/* Dialog Functions */
		static std::optional<std::string> OpenFile(const std::vector<std::pair<std::string, std::string>> filters);
		static std::string OpenFileDialog(const std::string& filter);
		static std::string OpenFolderDialog();
		static std::optional<std::string> SaveFile(const char* filter);
		static std::string SaveFileDialog(const std::string& name, const std::string& filter);
		static std::string SaveAsFileDialog(const std::string& filter);

		/* File Path Functions */
		static bool FileExists(const std::string& filename);
		static std::string GetPath(const std::string& filename);
		static std::string GetFilename(const std::string& path, bool addExtension = false);
		static std::string GetFileExtension(const std::string& path);
		static std::string GetRelativeAssetPath(const std::string& path);
		static void CreateFolder(const std::string& path);
		static void DeleteFolder(const std::string& path);
		static void RenameFolder(const std::string& path, const std::string& newPath);
		static bool FolderExists(const std::string& filename);

		/* File Functions */
		static bool CreateRawFile(const std::string& filename, const std::string& content);
		static bool CopySingleFile(const std::string& filename, const std::string& directory);
		static void RenameFile(const std::string& filename, const std::string& newFilename);
		static void RemoveFile(const std::string& filename);
		static const std::string Utils::BuildPath(std::initializer_list<std::reference_wrapper<std::string>> args);
		static std::vector<std::string> GetFolderList(const std::string& directory, bool asFullPaths);
		static std::vector<std::string> Utils::GetFileList(const std::string& directory, bool asFullPaths = false, bool recursive = false);
		static uint32_t CountSubDirectories(const std::string& directory);
		static std::string ReadFileToString(const std::string& filename);

		static void CreateProjectFileStructure(const std::string& rootFolder);

		/* System operations */
		static void OpenFileWithSystem(const std::string& file, const std::string& parameters = "");

		/* Hashing */
		static std::string HashFile(const std::string& file);

		/* String Convertors */
		static bool ToBool(std::string& value);
		static unsigned int ToUInt(std::string& value);
		static int ToInt(std::string& value);
		static float ToFloat(std::string& value);
		static Color ToColor(std::string& value);

		/* String Convertors */
		static std::string ToString(unsigned int value);
		static std::string ToString(int value);
		static std::string ToString(float value);
		static std::string ToString(Color& value);

		/* String Helpers */
		static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
		static std::vector<std::string> SplitString(std::string& str);

	private:
		static void SwapSlashes(std::string& path, const std::string& find, const std::string& replace);
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
		static std::string Utils::builds_path;
		static std::string Utils::cache_path;
		static std::string Utils::temp_path;
	};


}