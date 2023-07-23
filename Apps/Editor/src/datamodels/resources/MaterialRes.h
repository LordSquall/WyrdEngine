#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Material.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class MaterialRes : public Resource
	{
	public:
		MaterialRes(const std::filesystem::path& filepath, const UID& uid);
		~MaterialRes() = default;

		
		int Load() override;
		ResourceType GetType() override { return ResourceType::MATERIAL; }
		inline const std::shared_ptr<Material> GetMaterial() const { return _material; }

	private:
		std::shared_ptr<Material> _material;
	};
}