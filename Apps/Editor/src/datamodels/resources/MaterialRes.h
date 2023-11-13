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
		MaterialRes(const std::string& name, const UID& uid);
		~MaterialRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& object) override;

		IOResult Save(const std::string& filepath) override;

		void ResolveReferences() override;
		void Build(bool isRebuild = false) override;

		ResourceType GetType() override { return ResourceType::MATERIAL; }
		inline const std::shared_ptr<Material> GetMaterial() const { return _material; }


		void DrawProperties() override;

	private:
		std::shared_ptr<Material> _material;
		ShaderResRef _shaderResRef;
	};
}