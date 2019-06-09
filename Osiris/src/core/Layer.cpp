#include "osrpch.h"
#include "Layer.h"

namespace Osiris {

	Layer::Layer(const std::string& name)
		: _Name(name), _Enabled(true)
	{

	}

	Layer::~Layer()
	{
	}
}