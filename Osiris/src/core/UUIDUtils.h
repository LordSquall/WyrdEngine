#pragma once

/* external includes */
#include <crossguid/guid.hpp>

namespace Osiris::Editor
{
	typedef xg::Guid UUID;

	class UUIDUtils
	{
	public:
		static UUID Create();
	};
}