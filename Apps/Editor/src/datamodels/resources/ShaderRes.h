#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class ShaderRes : public Resource
	{
	public:
		ShaderRes(const std::filesystem::path& filepath, const UID& uid);
		~ShaderRes() = default;

		
		int Load() override;
		ResourceType GetType() override { return ResourceType::SHADER; }

	private:
		std::shared_ptr<Shader> _shader;
	};
}