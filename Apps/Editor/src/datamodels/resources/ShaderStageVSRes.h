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
	struct ShaderStageVSInputBinding
	{
		std::string type;
		std::string binding;
	};

	typedef std::map<std::string, ShaderStageVSInputBinding> ShaderStageVSInputMap;
	typedef std::map<std::string, ShaderStageVSInputBinding> ShaderStageVSOutputMap;

	class ShaderStageVSRes : public Resource
	{
	public:
		ShaderStageVSRes(const std::string& name, const UID& uid);
		~ShaderStageVSRes() = default;

		IOResult Load(const std::string & filepath) override;
		IOResult Load(const jsonxx::Object & obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::SHADERSTAGE_VS; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;

		bool Build();

		void DrawProperties() override;

		inline std::string GetSource() const { return _source; }

	private:
		std::string _source;

		ShaderStageVSInputMap _Inputs;
		ShaderStageVSOutputMap _Outputs;

		std::vector<ShaderResRef> _ownedShaders;
	};
}