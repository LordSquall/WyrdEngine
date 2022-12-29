#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/behaviour/ScriptedClass.h>

/* local project includes */
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"


namespace Wyrd::Editor
{
	class ScriptRes : public Resource
	{
	public:
		ScriptRes(const std::filesystem::path& filepath);

		~ScriptRes() = default;

		/* Resource overrides */
		bool Load() override;
		int GetType() override { return ResourceType::SCRIPT; }

	public:
		std::shared_ptr<Wyrd::ScriptedClass> Script;
		bool PendingReload;
	};
}