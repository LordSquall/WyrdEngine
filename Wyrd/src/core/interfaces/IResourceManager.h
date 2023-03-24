#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

namespace Wyrd
{
	/**
	 * @brief IResourceManager class (interface)
	 *
	 * Behaviour interface for all Resource Management functions.
	*/
	class WYRD_LIBRARY_API IResourceManager
	{
	public:
		virtual bool RegisterTexture(int uid) = 0;
	};
}