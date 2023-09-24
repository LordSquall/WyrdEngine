#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/Structures.h>

/* local includes */


/* external includes */


namespace Wyrd::Editor
{
	/**
	 * @brief Utils class
	 *
	 * Provides a number of static functions which handle mulitple utility operations
	*/
	class Utils 
	{
	public:
		Utils() = delete;
		~Utils() = default;

		/**
		 * @brief Get the absolute folder location of the editor resources folder 
		 * @return string - editor resource folder
		*/
		static std::string GetEditorResFolder();

		/* Dialog Functions */
		// TODO to move to the dialog service
		static std::optional<std::string> OpenFile(const std::vector<std::pair<std::string, std::string>> filters);
		static std::string OpenFileDialog(const std::string& filter);
		static std::string OpenFolderDialog();
		static std::optional<std::string> SaveFile(const std::vector<std::pair<std::string, std::string>> filters);
		static std::string SaveFileDialog(const std::string& name, const std::string& filter);
		static std::string SaveAsFileDialog(const std::string& filter);

		/**
		 * @brief Check if the file exists
		 * @param file - file to check
		 * @return bool - true if file exists
		*/
		static bool FileExists(const std::filesystem::path& file);

		/**
		 * @brief Create a new folder at the supplied path
		 * @param path -  path to create
		 * @return bool - true if folder was created
		*/
		static bool CreateFolder(const std::filesystem::path& path);

		/**
		 * @brief Delete a folder add all it's contents from the supplied path
		 * @param path -  path to delete
		 * @return bool - true if folder was deleted
		*/
		static bool DeleteFolder(const std::filesystem::path& path);


		static void RenameFolder(const std::string& path, const std::string& newPath);
		static bool FolderExists(const std::string& filename);

		/* File Functions */
		static bool CreateRawFile(const std::filesystem::path& filename, const std::string& content, bool overwrite = false);
		static bool CopySingleFile(const std::filesystem::path& filename, const std::filesystem::path& directory, bool overwrite = false);
		static void RenameFile(const std::filesystem::path& filename, const std::filesystem::path& newFilename);
		static void RemoveFile(const std::filesystem::path& filename);
		static const std::string BuildPath(std::initializer_list<std::reference_wrapper<std::string>> args);
		static std::vector<std::string> GetFolderList(const std::string& directory, bool asFullPaths);
		static std::vector<std::filesystem::path> GetFileList(const std::string& directory, bool asFullPaths = false, bool recursive = false);
		static uint32_t CountSubDirectories(const std::string& directory);
		static std::string ReadFileToString(const std::string& filename);

		/* System operations */
		static void OpenFileWithSystem(const std::filesystem::path& file, const std::string& parameters = "");
		static void SystemExecute(const std::string& command);

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
	};


}