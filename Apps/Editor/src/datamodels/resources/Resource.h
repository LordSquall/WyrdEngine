#pragma once

#include "osrpch.h"

#include "core/export.h"

namespace Osiris::Editor
{
	class Resource
	{
	public:
		Resource() : _uid(_nextUid)
		{
			_nextUid++;
		}

		~Resource() {}
		
		// Getters and Setters
		inline uint32_t GetUID() { return _uid; }

	private:
		uint32_t _uid = 0u;

		static uint32_t _nextUid;
	};
}