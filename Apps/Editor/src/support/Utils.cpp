#pragma once

#include "osrpch.h"
#include "Utils.h"
#include <shobjidl.h>
#include <shtypes.h>
#include <shellapi.h>
#include <corecrt_wstring.h>


namespace Osiris::Editor {

	std::string Utils::canonical_path = std::filesystem::current_path().generic_string();
	std::string Utils::asset_path = "/assets";

	Utils::Utils()
	{

	}	
	Utils::~Utils()
	{

	}

	void Utils::SetRootProjectFolder(const std::string& rootFolder)
	{
		canonical_path = rootFolder;
	}

	std::string Utils::GetAssetFolder()
	{
		return Utils::canonical_path + Utils::asset_path;
	}

	std::string Utils::GetEditorResFolder()
	{
		return std::filesystem::current_path().string();
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
							SwapSlashes(result,"\\", "/");
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
							SwapSlashes(result, "\\", "/");
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
									SwapSlashes(result, "\\", "/");
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
		std::string directory;
		const size_t last_slash_idx = filename.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = filename.substr(0, last_slash_idx);
		}
		return directory;
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

	std::string Utils::GetRelativeAssetPath(const std::string& path)
	{
		return path.substr(Utils::GetAssetFolder().length(), path.length() - Utils::GetAssetFolder().length());
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

	void Utils::CreateFolder(const std::string& path)
	{
		std::filesystem::create_directory(path);
	}

	void Utils::CreateProjectFileStructure(const std::string& rootFolder)
	{
		std::filesystem::create_directory(GetAssetFolder());
	}

	void Utils::OpenFileWithSystem(const std::string& path)
	{
		ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOW);
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

	void Utils::SwapSlashes(std::string& path, const std::string& find,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = path.find(find, pos)) != std::string::npos) {
			path.replace(pos, find.length(), replace);
			pos += replace.length();
		}
	}
}