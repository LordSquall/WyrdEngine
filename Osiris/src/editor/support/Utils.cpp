#pragma once

#include "osrpch.h"
#include "Utils.h"
#include <shobjidl.h>


namespace Osiris::Editor {
	std::string Utils::canonical_path = std::filesystem::current_path().generic_string();
	std::string Utils::asset_path = "/res";
	Utils::Utils()
	{

	}	
	Utils::~Utils()
	{

	}

	std::string Utils::GetAssetFolder()
	{
		OSR_INFO(Utils::canonical_path + Utils::asset_path);
		if (std::filesystem::exists(Utils::canonical_path + Utils::asset_path)) {
			OSR_INFO("true");
			Utils::asset_path = Utils::canonical_path + Utils::asset_path;
		}
		return Utils::asset_path;
	}

	std::string Utils::OpenFileDialouge() {
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

	std::string Utils::OpenFolderDialouge() {
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

	void Utils::SwapSlashes(std::string& path, const std::string& find,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = path.find(find, pos)) != std::string::npos) {
			path.replace(pos, find.length(), replace);
			pos += replace.length();
		}
	}
}