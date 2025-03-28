#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class ShaderStageRes : public Resource
	{
	public:
		ShaderStageRes(const std::string& name, const UID& uid);
		~ShaderStageRes() = default;

		IOResult Load(const std::string & filepath) override;
		IOResult Load(const jsonxx::Object & obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::SHADERSTAGE; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;

		void DrawProperties() override;

		inline std::string GetSource() const { return _source; }

	private:
		std::string _source;

		std::vector<ShaderResRef> _ownedShaders;
	};
}