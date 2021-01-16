#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/behaviour/ScriptedClass.h>

/* local project includes */
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"


namespace Osiris::Editor
{
	class ScriptRes : public Resource
	{
	public:
		ScriptRes(const std::string& path);

		~ScriptRes() = default;

		/* Resource overrides */
		bool Load() override;
		int GetType() override { return ResourceType::SCRIPT; }

	public:
		std::shared_ptr<Osiris::ScriptedClass> Script;
	};
}