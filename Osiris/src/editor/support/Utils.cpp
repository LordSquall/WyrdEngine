#pragma once

#include "osrpch.h"
#include "Utils.h"
#include <shobjidl.h>

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
		OSR_INFO(Utils::canonical_path + Utils::asset_path);
		if (std::filesystem::exists(Utils::canonical_path + Utils::asset_path)) {
			return Utils::canonical_path + Utils::asset_path;
		}
		return Utils::asset_path;
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

	std::string Utils::SaveFileDialog(const std::string& filter)
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
				// Show the Open dialog box.
				hr = pFileSave->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
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

	void Utils::CreateProjectFileStructure(const std::string& rootFolder)
	{
		std::filesystem::create_directory(GetAssetFolder());
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