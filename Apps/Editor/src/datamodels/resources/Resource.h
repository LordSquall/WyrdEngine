#pragma once

#include "osrpch.h"

#include "core/export.h"

namespace Osiris::Editor
{
	class Resource
	{
	public:
		Resource() : _resourceID(_nextResourceID)
		{
			_nextResourceID++;
		}

		~Resource() {}
		
		// Getters and Setters
		inline uint32_t GetResourceID() { return _resourceID; }

	private:
		uint32_t _resourceID = 0u;

		static uint32_t _nextResourceID;
	};
}