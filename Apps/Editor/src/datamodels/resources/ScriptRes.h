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
		ScriptRes(const std::filesystem::path& filepath, const UID& uid);

		~ScriptRes() = default;

		/* Resource overrides */
		int Load() override;
		ResourceType GetType() override { return ResourceType::SCRIPT; }

	public:
		std::shared_ptr<Wyrd::ScriptedClass> Script;
		bool PendingReload;
	};
}