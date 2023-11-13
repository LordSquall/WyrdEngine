#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "core/export.h"
#include "Resource.h"
#include "support/Utils.h"
#include "datamodels/resources/ResourceTypes.h"
#include "datamodels/resources/ShaderStageRes.h"

typedef unsigned char BYTE;

namespace Wyrd::Editor
{
	class ShaderRes : public Resource
	{
	public:
		ShaderRes(const std::string& name, const UID& uid);
		~ShaderRes() = default;

		IOResult Load(const std::string& filepath) override;
		IOResult Load(const jsonxx::Object& obj) override;
		
		IOResult Save(const std::string& filepath) override;

		void ResolveReferences() override;
		void Build(bool isRebuild = false) override;

		ResourceType GetType() override { return ResourceType::SHADER; }

		void DrawProperties() override;

		inline const Wyrd::UID& GetVertexShaderStageID() const { return _VertexShaderStageId; }
		inline void SetVertexShaderStageID(const Wyrd::UID& uid) { _VertexShaderStageId = uid; }

		inline const Wyrd::UID& GetFragmentShaderStageID() const { return _FragmentShaderStageId; }
		inline void SetFragmentShaderStageID(const Wyrd::UID& uid) { _FragmentShaderStageId = uid; }

		inline std::shared_ptr<Shader> GetShader() const { return _shader; }

		inline std::shared_ptr<ShaderStageRes> GetVertexStage() const { return _VertexStage; }
		inline std::shared_ptr<ShaderStageRes> GetFragmentStage() const { return _FragmentStage; }

	private:
		std::shared_ptr<Shader> _shader;

		Wyrd::UID _VertexShaderStageId;
		Wyrd::UID _FragmentShaderStageId;

		ShaderStageResRef _VertexStage;
		ShaderStageResRef _FragmentStage;

		bool _isBuilt;
	};
}