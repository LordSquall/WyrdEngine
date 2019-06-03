#pragma once
#include "core/export.h"
#include "osrpch.h"

namespace Osiris::Editor
{
	class OSIRIS_API Utils
	{
	public:
		Utils();
		~Utils();

		std::filesystem::path GetAssetFolder();

	};

}