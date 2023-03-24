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
		INT32 width;
		INT32 height;
	};

}