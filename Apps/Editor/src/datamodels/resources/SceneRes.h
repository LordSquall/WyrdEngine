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
		SceneRes(const std::string& name, const UID& uid);
		~SceneRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::SCENE; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;
	};
}