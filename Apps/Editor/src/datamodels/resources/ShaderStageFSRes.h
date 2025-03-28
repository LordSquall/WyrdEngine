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
	struct ShaderStageFSInputBinding
	{
		std::string type;
		std::string binding;
	};

	typedef std::map<std::string, ShaderStageFSInputBinding> ShaderStageFSInputMap;
	typedef std::map<std::string, ShaderStageFSInputBinding> ShaderStageFSOutputMap;

	class ShaderStageFSRes : public Resource
	{
	public:
		ShaderStageFSRes(const std::string& name, const UID& uid);
		~ShaderStageFSRes() = default;

		IOResult Load(const std::string & filepath) override;
		IOResult Load(const jsonxx::Object & obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::SHADERSTAGE_FS; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;

		bool Build();

		void DrawProperties() override;

		inline std::string GetSource() const { return _source; }

	private:
		std::string _source;

		ShaderStageFSInputMap _Inputs;
		ShaderStageFSOutputMap _Outputs;

		std::vector<ShaderResRef> _ownedShaders;
	};
}