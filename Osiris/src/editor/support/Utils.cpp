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

	HRESULT Utils::OpenFolderDialouge() {
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
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
							MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
		return hr;
	}
}