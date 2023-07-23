#include "wyrdpch.h"
#include "core/Application.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Material.h"

namespace Wyrd
{
	Material* Material::Create(const MaterialDesc& desc)
	{
		Material* newMaterial = new Material();

		newMaterial->SetUID(desc.resource.guid);
		newMaterial->SetName(desc.resource.name);

		newMaterial->SetShader(Application::Get().GetResources().Shaders[desc.shaderName]);

		for (auto& input : desc.inputMap)
		{
			newMaterial->AddInputBinding(input.first, input.second.type, input.second.binding);
		}

		return newMaterial;
	}

	void Material::BindViewMatrix(const glm::mat4& view)
	{
		_shader->SetMatrix("u_view", view);
	}

	void Material::BindProjectionMatrix(const glm::mat4& projection)
	{
		_shader->SetMatrix("u_projection", projection);
	}

	void Material::BindShader()
	{
		_shader->Bind();
	}

	void Material::Bind(const BasePropMapRef* propMap)
	{
		if (*propMap)
		{
			for (auto& input : _inputs)
			{
				if (input.second.type == "Color")
				{
					PropColor* color = (PropColor*)(*propMap)->at(input.first).get();
					if (color != nullptr)
					{
						_shader->SetUniformColor(input.second.binding, color->Value);
					}
				}

				if (input.second.type == "Texture")
				{
					PropTexture* textureProp = (PropTexture*)(*propMap)->at(input.first).get();
					Texture* texture = textureProp->Get<Texture*>();
					if (texture != nullptr)
					{
						texture->Bind();
					}
				}
			}
		}
	}


	void Material::AddInputBinding(const std::string& name, const std::string& type, const std::string& binding)
	{
		(_inputs)[name] = { type, binding };
	}
}