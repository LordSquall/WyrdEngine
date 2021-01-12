#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/behaviour/ScriptedClass.h>

/* local project includes */
#include "Resource.h"
#include "support/Utils.h"


namespace Osiris::Editor
{
	class ScriptRes : public Resource
	{
	public:
		ScriptRes(const std::string& path);

		~ScriptRes() = default;

		/* Resource overrides */
		bool Load() override;

		/* Getters and setters */
		inline const std::string& GetPath() { return _path; }
		inline void SetPath(const std::string& path) { _path = path; }

	public:
		std::shared_ptr<Osiris::ScriptedClass> Script;

	private:
		std::string _path;
	};
}