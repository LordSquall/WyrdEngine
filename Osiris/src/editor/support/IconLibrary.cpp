#pragma once

#include "osrpch.h"

#include "IconLibrary.h"

#include "core/Application.h"

#include <nlohmann/json.hpp>
#include <SOIL.h>

using namespace nlohmann;

namespace Osiris
{
	IconLibrary::IconLibrary()
	{
	}

	IconLibrary::~IconLibrary()
	{

	}


	bool IconLibrary::AddIconsFromFile(std::string& filepath)
	{
		return true;
	}


	Icon& IconLibrary::GetIconFromFileExtension(const std::filesystem::path& path)
	{
		Icon icon;

		std::string extension = path.extension().string();

		if (extension.compare(".txt") == 0)
		{
			icon = m_Icons["text_file"];
		}
		else
		{
			icon = m_Icons["unknown_file"];
		}

		return icon;
	}
}