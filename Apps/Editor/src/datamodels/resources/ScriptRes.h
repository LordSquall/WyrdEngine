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
		ScriptRes(const std::string& name, const UID& uid);
		~ScriptRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::SCRIPT; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;

	public:
		std::shared_ptr<Wyrd::ScriptedClass> Script;
		bool PendingReload;
	};
}