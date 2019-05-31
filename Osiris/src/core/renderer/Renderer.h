#pragma once

#include "osrpch.h"
#include "events/Event.h"

namespace Osiris
{
	class Renderer
	{
	public:
		virtual ~Renderer() {};

		static Renderer* Create(); 
		
	};
}