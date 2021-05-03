#pragma once


/* local includes */
#include <wyrdpch.h>
#include "core/UID.h"

namespace Wyrd
{
	UID UIDUtils::Create()
	{
		return xg::newGuid();
	}
}