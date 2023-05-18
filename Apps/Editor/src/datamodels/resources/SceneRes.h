#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include "core/export.h"

/* local includes */
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

namespace Wyrd::Editor
{
	class SceneRes : public Resource
	{
	public:
		SceneRes(const std::filesystem::path& filepath, const UID& uid);

		~SceneRes() = default;

		/* Resource overrides */
		int Load() override;
		ResourceType GetType() override { return ResourceType::SCENE; }
	};
}