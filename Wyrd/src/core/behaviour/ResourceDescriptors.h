#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/UID.h"

namespace Wyrd
{
	struct ManagedTextureDesc
	{
		void* nativePtr;
		std::array<unsigned char, 16> guid;
		int32_t width;
		int32_t height;
	};

}