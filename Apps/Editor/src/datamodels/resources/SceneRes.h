#pragma once

/* core osiris includes */
#include <osrpch.h>
#include "core/export.h"

/* local includes */
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

namespace Osiris::Editor
{
	class SceneRes : public Resource
	{
	public:
		SceneRes(const std::string& path);

		~SceneRes() = default;

		/* Resource overrides */
		bool Load() override;
		int GetType() override { return ResourceType::SCENE; }
	};
}