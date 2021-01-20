#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/UID.h>

/* local includes */

namespace Osiris::Editor
{
	class Resource
	{
	public:
		Resource(const std::string& name, const std::string& path) : _name(name), _path(path)
		{
		}

		virtual ~Resource() = default;

		virtual bool Load() = 0;

		virtual int GetType() = 0;

		// Getters and Setters
		inline const std::string& GetName() { return _name; }
		inline void SetName(const std::string& name) { _name = name; }
		inline const std::string& GetPath() { return _path; }
		inline void SetPath(const std::string& path) {_path = path; }
		inline const UID GetResourceID() { return _resourceID; }
		inline void SetResourceID(const UID uuid) { _resourceID = uuid; }


	protected:
		std::string _name;
		std::string _path;
		UID _resourceID;
	};
}