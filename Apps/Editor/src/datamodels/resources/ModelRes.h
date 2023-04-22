#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Texture.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class ModelRes : public Resource
	{
	public:
		ModelRes(const std::filesystem::path& filepath);

		~ModelRes() = default;

		// Resource overrides
		bool Load() override;
		int GetType() override { return ResourceType::MODEL; }

		// Getters and Setters
		inline std::shared_ptr<Mesh> GetMesh() { return _mesh; }
		inline bool IsLoaded() { return _loaded; }
		inline void SetLoaded(bool loaded) { _loaded = loaded; }

	private:
		bool _loaded;
		std::shared_ptr<Mesh> _mesh;
	};
}