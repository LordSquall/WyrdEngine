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

		newMaterial->SetShaderUID(desc.shaderUID);
		newMaterial->SetShader(Application::Get().GetResources().Shaders[desc.shaderUID]);

		for (auto& input : desc.inputMap)
		{
			//newMaterial->AddInputBinding(input.first, input.second.type, input.second.binding);
		}

		return newMaterial;
	}

	void Material::BindModelMatrix(const glm::mat4& view)
	{
		_shader->SetMatrix("u_model", view);
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
			unsigned int _texSlotIdx = 0;
			for (auto& input : _inputs)
			{
				if ((**propMap).find(input.first) != (**propMap).end())
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
							texture->Bind(_texSlotIdx);
							_texSlotIdx++;
						}
					}

					if (input.second.type == "Vec2")
					{
						PropVec2* vec2Prop = (PropVec2*)(*propMap)->at(input.first).get();
						if (vec2Prop != nullptr)
						{
							glm::vec2 v = glm::vec2(vec2Prop->Value.x, vec2Prop->Value.y);
							_shader->SetUniformVec2(input.second.binding, v);
						}
					}

					if (input.second.type == "Vec3")
					{
						PropVec3* vec3Prop = (PropVec3*)(*propMap)->at(input.first).get();
						if (vec3Prop != nullptr)
						{
							glm::vec3 v = glm::vec3(vec3Prop->Value.x, vec3Prop->Value.y, vec3Prop->Value.z);
							_shader->SetUniformVec3(input.second.binding, v);
						}
					}
				}
			}
		}
	}


	void Material::AddInputBinding(const std::string& name, const std::string& type, const std::string& binding, const jsonxx::Object& defaultData)
	{
		(_inputs)[name] = { type, binding, defaultData };
	}
}