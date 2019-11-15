#pragma once

#include "osrpch.h"

#include "core/export.h"
#include "Resource.h"

#include "editor/support/Utils.h"

namespace Osiris::Editor
{
	class SceneRes : public Resource
	{
	public:
		SceneRes(const std::string& path);

		~SceneRes() {}

		/* Getters and setters */
		inline const std::string& GetPath() { return _path; }
		inline void SetPath(const std::string& path) { _path = path; }

		inline const std::string& GetName() { return _name; }

	private:
		std::string _path;
		std::string _name;
	};
}