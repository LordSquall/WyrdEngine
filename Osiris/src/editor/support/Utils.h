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
		std::string OpenFileDialouge();
		std::string Utils::OpenFolderDialouge();


	private:
		void SwapSlashes(std::string& path, const std::string& find,
			const std::string& replace);
		static std::string Utils::canonical_path;
		static std::string Utils::asset_path;
	};


}