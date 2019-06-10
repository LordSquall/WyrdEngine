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
		HRESULT OpenFolderDialouge();

	private:
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
	};


}