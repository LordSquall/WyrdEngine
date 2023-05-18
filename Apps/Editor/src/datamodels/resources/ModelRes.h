#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

/* core includes */
#include <core/renderer/Mesh.h>

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class ModelRes : public Resource
	{
	public:
		ModelRes(const std::filesystem::path& filepath, const UID& uid);

		~ModelRes() = default;

		// Resource overrides
		int Load() override;
		ResourceType GetType() override { return ResourceType::MODEL; }

		// Getters and Setters
		inline std::shared_ptr<Mesh> GetMesh() { return _mesh; }

	private:
		std::shared_ptr<Mesh> _mesh;
	};
}