#pragma once

#include "osrpch.h"
#include "Utils.h"


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
}