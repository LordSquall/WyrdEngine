#pragma once

#include "osrpch.h"

#include "core/export.h"
#include "Resource.h"

#include "support/Utils.h"

#include "behaviour/ScriptedObjectTemplate.h"

namespace Osiris::Editor
{
	class LuaRef;

	class ScriptRes : public Resource
	{
	public:
		ScriptRes(const std::string& path);

		~ScriptRes() {}

		/* Getters and setters */
		inline const std::string& GetPath() { return _path; }
		inline void SetPath(const std::string& path) { _path = path; }

		inline const std::string& GetName() { return _name; }

		void Reload();
		
	public:
		std::shared_ptr<ScriptedObjectTemplate> ScriptedObjectTemplate;

	private:
		std::string _path;
		std::string _name;
	};
}