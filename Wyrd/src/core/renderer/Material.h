#pragma once

/* local includes */
#include "core/export.h"
#include "core/Structures.h"
#include "core/renderer/Shader.h"
#include "properties/BaseProp.h"

/* external includes */
#include <string>

namespace Wyrd
{
	struct InputBinding
	{
		std::string type;
		std::string binding;
	};

	typedef std::map<std::string, InputBinding> MaterialInputMap;


	struct MaterialDesc
	{
		ResourceDesc resource;
		UID shaderUID;
		MaterialInputMap inputMap;

		MaterialDesc() : resource() { }
	};

	class WYRD_LIBRARY_API Material : public ResourceBase
	{
	public:
		Material() {};
		Material(std::shared_ptr<Material> material);

		void BindModelMatrix(const glm::mat4& modelMat);
		void BindViewMatrix(const glm::mat4& view);
		void BindProjectionMatrix(const glm::mat4& projection);

		void BindShader();
		void Bind(const BasePropMapRef* propMap);

		void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }
		inline std::shared_ptr<Shader> GetShader() const { return _shader; }

		void SetShaderUID(const UID& id) { _shaderUID = id; }
		inline const UID& GetShaderUID() const { return _shaderUID; }

		void AddInputBinding(const std::string& name, const std::string& type, const std::string& binding);


		inline const MaterialInputMap& GetInputPropertyList() const { return _inputs; }

	private:
		std::shared_ptr<Shader> _shader;
		UID _shaderUID;
		MaterialInputMap _inputs;
		bool _isShaderLoaded;

	public:
		static Material* Create(const MaterialDesc& desc);
	};
}