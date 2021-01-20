#pragma once


/* local includes */
#include <osrpch.h>
#include "core/UID.h"

namespace Osiris
{
	UID UIDUtils::Create()
	{
		return xg::newGuid();
	}
}