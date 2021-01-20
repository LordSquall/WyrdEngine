#pragma once

/* external includes */
#include <crossguid/guid.hpp>

namespace Osiris
{
	typedef xg::Guid UID;

	class UIDUtils
	{
	public:
		static UID Create();
	};
}