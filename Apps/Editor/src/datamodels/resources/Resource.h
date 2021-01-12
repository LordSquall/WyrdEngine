#pragma once

#include "osrpch.h"

#include "core/export.h"

namespace Osiris::Editor
{
	class Resource
	{
	public:
		Resource(const std::string& name) : _resourceID(_nextResourceID), _name(name)
		{
			_nextResourceID++;
		}

		virtual ~Resource() = default;

		virtual bool Load() = 0;
		
		// Getters and Setters
		inline const std::string& GetName() { return _name; }
		inline uint32_t GetResourceID() { return _resourceID; }

	protected:
		std::string _name;
		uint32_t _resourceID = 0u;

	private:
		static uint32_t _nextResourceID;
	};
}