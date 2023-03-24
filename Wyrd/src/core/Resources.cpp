#include "wyrdpch.h"

#include "Resources.h"

namespace Wyrd {

	Resources* Resources::_Instance = nullptr;

	Resources::Resources()
	{
		_Instance = this;
	}
}