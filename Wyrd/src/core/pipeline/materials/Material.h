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
	class WYRD_LIBRARY_API Material
	{
	public:
		struct InputBinding
		{
			std::string type;
			std::string binding;
		};

		typedef std::map<std::string, InputBinding> InputMap;

	public:
		Material() {};

		void BindViewMatrix(const glm::mat4& view);
		void BindProjectionMatrix(const glm::mat4& projection);

		void BindShader();
		void Bind(const BasePropMapRef* propMap);

		void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }

		void AddInputBinding(const std::string& name, const std::string& type, const std::string& binding);

		inline const InputMap& GetInputPropertyList() const { return _inputs; }

	private:
		std::shared_ptr<Shader> _shader;
		InputMap _inputs;
	};
}