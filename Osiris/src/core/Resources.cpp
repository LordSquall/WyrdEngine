#include "osrpch.h"

#include "Resources.h"

namespace Osiris {

	Resources* Resources::_Instance = nullptr;

	Resources::Resources()
	{
		_Instance = this;
	}
}