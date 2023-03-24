#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/UID.h>

/* local includes */

#ifndef WYRD_PLATFORM_LINUX
typedef unsigned char BYTE;
#endif

namespace Wyrd::Editor
{
	class Resource
	{
	public:
		Resource(const std::string& name, const std::filesystem::path& path) : _name(name), _path(path)
		{
		}

		virtual ~Resource() = default;

		virtual bool Load() = 0;

		virtual int GetType() = 0;

		// Getters and Setters
		inline const std::string& GetName() { return _name; }
		inline void SetName(const std::string& name) { _name = name; }
		inline const std::filesystem::path& GetPath() { return _path; }
		inline void SetPath(const std::string& path) {_path = path; }
		inline const UID GetResourceID() { return _resourceID; }
		inline void SetResourceID(const UID uuid) { _resourceID = uuid; }


	protected:
		std::string _name;
		std::filesystem::path _path;
		UID _resourceID;
	};
}