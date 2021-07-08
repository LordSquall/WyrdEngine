#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>
#include <core/Log.h>

/* local includes */
#include "Utils.h"

/* external includes */
#include <shobjidl.h>
#include <shtypes.h>
#include <shellapi.h>
#include <corecrt_wstring.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Wyrd::Editor {

	Utils::Utils()
	{

	}	
	Utils::~Utils()
	{

	}

	std::string Utils::GetEditorResFolder()
	{
		return std::filesystem::current_path().string() + "/res/";
	}

	std::optional<std::string> Utils::OpenFile(const std::vector<std::pair<std::string, std::string>> filters)
	{
		/* build the filter string */
		std::stringstream filterString;

		for (auto& filter : filters)
		{
			filterString << filter.first.c_str();
			filterString << '\0';
			filterString << filter.second.c_str();
			filterString << '\0';
		}

		const std::string tmp = filterString.str();
		const char* cstr = tmp.c_str();

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindowPointer());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = cstr;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		return std::nullopt;
	}

	std::string Utils::OpenFileDialog(const std::string& filter) {

		// Template modified from https://docs.microsoft.com/en-us/windows/desktop/learnwin32/example--the-open-dialog-box
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		std::string result;
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog *pFileOpen;
						
			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			if (SUCCEEDED(hr))
			{

				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{						
							char str[MAX_PATH];
							wcstombs(str, pszFilePath, MAX_PATH);
							result = str;
							SwapSlashes(result,"\\", "\\");
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return result;
	}

	std::string Utils::OpenFolderDialog() {
		// Template modified from https://docs.microsoft.com/en-us/windows/desktop/learnwin32/example--the-open-dialog-box
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		std::string result;
		if (SUCCEEDED(hr))
		{
			IFileDialog *pFileOpen;
			FILEOPENDIALOGOPTIONS fos = FOS_PICKFOLDERS;


			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
			pFileOpen->SetOptions(fos);
			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							char str[MAX_PATH];
							wcstombs(str, pszFilePath, MAX_PATH);
							result = str;
							SwapSlashes(result, "\\", "\\");
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return result;
	}


	std::optional<std::string> Utils::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindowPointer());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		return std::nullopt;
	}

	std::string Utils::SaveFileDialog(const std::string& name, const std::string& filter)
	{
		// Template modified from https://docs.microsoft.com/en-us/windows/desktop/learnwin32/example--the-open-dialog-box
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		std::string result;
		if (SUCCEEDED(hr))
		{
			IFileSaveDialog *pFileSave;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
				IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

			if (SUCCEEDED(hr))
			{
				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order 
				// not to override existing options.
				hr = pFileSave->GetOptions(&dwFlags);
				if (SUCCEEDED(hr))
				{
					// In this case, get shell items only for file system items.
					hr = pFileSave->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
					if (SUCCEEDED(hr))
					{
						// Set the default extension to be ".doc" file.
						std::wstring stemp = std::wstring(filter.begin(), filter.end());
						hr = pFileSave->SetDefaultExtension(stemp.c_str());
								
						// Show the Open dialog box.
						hr = pFileSave->Show(NULL);

						// Get the file name from the dialog box.
						if (SUCCEEDED(hr))
						{
							IShellItem* pItem;
							hr = pFileSave->GetResult(&pItem);
							if (SUCCEEDED(hr))
							{
								PWSTR pszFilePath;
								hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

								// Display the file name to the user.
								if (SUCCEEDED(hr))
								{
									char str[MAX_PATH];
									wcstombs(str, pszFilePath, MAX_PATH);
									result = str;
									SwapSlashes(result, "\\", "\\");
									CoTaskMemFree(pszFilePath);
								}
								pItem->Release();

							}
						}
					}
				}

				pFileSave->Release();
			}
			CoUninitialize();
		}
		return result;
	}

	std::string Utils::SaveAsFileDialog(const std::string& filter)
	{
		return "not yet implemented";
	}

	bool Utils::FileExists(const std::string& filename)
	{
		return std::filesystem::exists(filename);
	}

	std::string Utils::GetPath(const std::string& filename)
	{
		std::filesystem::path path(filename);
		return path.parent_path().string();
	}

	std::string Utils::GetFilename(const std::string& path, bool addExtension)
	{
		if (addExtension == true)
		{
			return std::filesystem::path(path).filename().string();
		}
		else
		{
			return std::filesystem::path(path).stem().string();
		}
	}

	std::string Utils::GetFileExtension(const std::string& path)
	{
		return std::filesystem::path(path).extension().string();
	}


	std::vector<std::string> Utils::GetFolderList(const std::string& directory, bool asFullPaths)
	{
		std::vector<std::string> r;
		for (auto& p : std::filesystem::directory_iterator(directory))
			if (p.is_directory())
				if (asFullPaths == true)
				{
					r.push_back(p.path().string());
				}
				else
				{
					r.push_back(p.path().filename().string());
				}
		return r;
	}

	std::vector<std::string> Utils::GetFileList(const std::string& directory, bool asFullPaths, bool recursive)
	{
		std::vector<std::string> r;
		if(recursive  == true)
		{
			for (auto& p : std::filesystem::recursive_directory_iterator(directory))
				if (!p.is_directory())
					if (asFullPaths == true)
					{
						r.push_back(p.path().string());
					}
					else
					{
						r.push_back(p.path().filename().string());
					}
		}
		else
		{
			for (auto& p : std::filesystem::directory_iterator(directory))
				if (!p.is_directory())
					if (asFullPaths == true)
					{
						r.push_back(p.path().string());
					}
					else
					{
						r.push_back(p.path().filename().string());
					}
		}
		return r;
	}


	uint32_t Utils::CountSubDirectories(const std::string& directory)
	{
		using std::filesystem::directory_iterator;
		using fp = bool (*)(const std::filesystem::path&);
		return std::count_if(directory_iterator(directory), directory_iterator{}, (fp)std::filesystem::is_directory);
	}

	std::string Utils::ReadFileToString(const std::string& filename)
	{
		std::ifstream t(filename);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		return str;
	}

	void Utils::CreateFolder(const std::string& path)
	{
		std::filesystem::create_directory(path);
	}

	void Utils::DeleteFolder(const std::string& path)
	{
		std::filesystem::remove_all(path);
	}

	void Utils::RenameFolder(const std::string& path, const std::string& newPath)
	{
		std::filesystem::path p(path);
		std::filesystem::rename(path, newPath);
	}

	bool Utils::FolderExists(const std::string& filename)
	{
		return std::filesystem::is_directory(std::filesystem::path(filename));
	}

	bool Utils::CreateRawFile(const std::string& filename, const std::string& content)
	{
		std::ofstream file(filename);
		file << content;
		file.close();

		return true;
	}

	bool Utils::CopySingleFile(const std::string& filename, const std::string& directory)
	{
		std::filesystem::copy(filename, directory);
		return true;
	}

	void Utils::RenameFile(const std::string& filename, const std::string& newFilename)
	{
		std::filesystem::rename(filename, newFilename);
	}

	void Utils::RemoveFile(const std::string& filename)
	{
		std::filesystem::remove(filename);
	}

	const std::string Utils::BuildPath(std::initializer_list<std::reference_wrapper<std::string>> args) {
		std::ostringstream stream; 
		for (auto arg : args) {
			stream << arg.get();
		}

		return stream.str();
	}

	void Utils::OpenFileWithSystem(const std::string& path, const std::string& parameters)
	{
		ShellExecuteA(NULL, "open", path.c_str(), parameters.c_str(), NULL, SW_SHOW);
	}

	/* Hashing */
	std::string Utils::HashFile(const std::string& file)
	{
		return std::string("");
	}


	bool Utils::ToBool(std::string& value)
	{
		return value == "0" ? false : true;
	}

	unsigned int Utils::ToUInt(std::string& value)
	{
		return std::stoul(value, NULL, 10);
	}

	int Utils::ToInt(std::string& value)
	{
		return std::stol(value, NULL, 10);
	}

	float Utils::ToFloat(std::string& value)
	{
		return std::stof(value);
	}

	Color Utils::ToColor(std::string& value)
	{
		auto tokens = Utils::SplitString(value);
		return { Utils::ToFloat(tokens[0]), Utils::ToFloat(tokens[1]), Utils::ToFloat(tokens[2]), Utils::ToFloat(tokens[3]) };
	}

	std::string Utils::ToString(unsigned int value)
	{
		return std::to_string(value);
	}

	std::string Utils::ToString(int value)
	{
		return std::to_string(value);
	}

	std::string Utils::ToString(float value)
	{
		return std::to_string(value);
	}
	
	std::string Utils::ToString(Color& color)
	{
		return Utils::ToString(color.r) + ',' + Utils::ToString(color.g) + ',' + Utils::ToString(color.b) + ',' + Utils::ToString(color.a);
	}

	std::string Utils::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
		return str;
	}


	std::vector<std::string> Utils::SplitString(std::string& str)
	{
		std::vector<std::string> result;
		std::stringstream ss(str);

		while (ss.good())
		{
			std::string substr;
			std::getline(ss, substr, ',');
			result.push_back(substr);
		}
		return result;
	}

	void Utils::SwapSlashes(std::string& path, const std::string& find,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = path.find(find, pos)) != std::string::npos) {
			path.replace(pos, find.length(), replace);
			pos += replace.length();
		}
	}
}