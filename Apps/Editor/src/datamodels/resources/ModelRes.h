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
		ModelRes(const std::string& name, const UID& uid);
		~ModelRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& obj) override;

		IOResult Save(const std::string& filepath) override;

		ResourceType GetType() override { return ResourceType::MODEL; }
		const std::string GetTypeTag() override;
		const Icon& GetThumbnail() override;

		// Getters and Setters
		inline std::shared_ptr<Mesh> GetMesh() { return _mesh; }

	private:
		std::shared_ptr<Mesh> _mesh;
	};
}