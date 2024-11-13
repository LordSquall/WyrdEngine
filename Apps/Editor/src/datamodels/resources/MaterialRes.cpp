/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>
#include <core/renderer/Material.h>

/* local includes */
#include "MaterialRes.h"
#include "ShaderRes.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/CustomComponentSerialisationFunctions.h"

/* external includes */
#include <jsonxx.h>

namespace Wyrd::Editor
{
	MaterialRes::MaterialRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{

	}
	
	Resource::IOResult MaterialRes::Load(const std::string& filepath)
	{
		Resource::IOResult result = IOResult::Unknown;
		jsonxx::Object o;
		std::ifstream f(filepath);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				_path = filepath;

				result = Load(o);
			}
			else
			{
				result = FileMalformed;
			}
		}

		return result;
	}

	Resource::IOResult MaterialRes::Load(const jsonxx::Object& object)
	{
		/* assign the internal material object */
		if (_material == nullptr)
		{
			_material = std::make_shared<Material>();
			Application::Get().GetResources().Materials[_resourceID] = _material;
		}

		/* name */
		std::string name = object.get<jsonxx::String>("name", "default");
		_material->SetName(name);
		_material->SetUID(_resourceID);

		/* export settings */
		if (object.has<jsonxx::Object>("shader"))
		{
			jsonxx::Object shaderObject = object.get<jsonxx::Object>("shader");

			/* shader id */
			UID shaderResId = UID(shaderObject.get<jsonxx::String>("resourceId", "default"));
			_material->SetShaderUID(shaderResId);

			/* check for shader bindings */
			if (shaderObject.has<jsonxx::Array>("inputs"))
			{
				jsonxx::Array inputsArray = shaderObject.get<jsonxx::Array>("inputs");

				for (size_t i = 0; i < inputsArray.size(); i++)
				{
					jsonxx::Object inputObject = inputsArray.get<jsonxx::Object>((int)i);

					std::string name = inputObject.get<jsonxx::String>("name");
					std::string type = inputObject.get<jsonxx::String>("type");
					std::string binding = inputObject.get<jsonxx::String>("binding");

					jsonxx::Object defaultData = inputObject.get<jsonxx::Object>("default", jsonxx::Object());

					_material->AddInputBinding(name, type, binding, defaultData);
				}
			}

			_isLoaded = true;
		}

		return Success;
	}

	Resource::IOResult MaterialRes::Save(const std::string& filepath)
	{
		/* construct into a json file */
		jsonxx::Object o;

		/* name */
		o << "name" << _name;

		{
			jsonxx::Object shaderObj;

			shaderObj << "resourceId" << _shaderResRef->GetResourceID();

			jsonxx::Array inputArray;
			for (auto& [name, prop] : _material->GetInputPropertyList())
			{
				jsonxx::Object inputObj;
				inputObj << "name" << name;
				inputObj << "type" << prop.type;
				inputObj << "binding" << prop.binding;

				inputArray << inputObj;
			}
			shaderObj << "inputs" << inputArray;

			o << "shader" << shaderObj;
		}

		std::ofstream out(filepath);
		out << o.json();
		out.close();
		return NotImplemented;
	}

	void MaterialRes::ResolveReferences()
	{
		std::shared_ptr<ResourceService> resourceService = ServiceManager::Get<ResourceService>();

		/* Resolve Resources */
		_shaderResRef = resourceService->GetResourceByID<ShaderRes>(_material->GetShaderUID());
		if (_shaderResRef)
		{
			_shaderResRef->ResolveReferences();
			
			/* Hook up 'upward' resource chains */
			_shaderResRef->AddListeningResource(this->shared_from_this());

			/* apply any resources */
			_material->SetShader(_shaderResRef->GetShader());
		}	
	}

	void MaterialRes::Build(bool isBuild) {}

	void MaterialRes::DrawProperties()
	{
		ImGui::Text("Material Name: %s", _name.c_str());

		if (_material != nullptr && _material->GetShader() != nullptr)
		{
			ImGui::Text("Shader: %s", _material->GetShader()->GetName().c_str());
		}
		else
		{
			ImGui::Text("Shader: <unbound>");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_SHADER))
			{
				UID* shaderUID = (UID*)payload->Data;
				_material->SetShaderUID(*shaderUID);
				ResolveReferences();
				Save(_path.string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SeparatorText("Bindings");

		ImVec2 pos = ImGui::GetCursorPos();
		static int selected;
		float width = ImGui::GetContentRegionAvail().x;


		MaterialInputMap& inputMap = const_cast<MaterialInputMap & >(_material->GetInputPropertyList());

		// selectable list
		int n = 0;
		for (auto& [k, v] : inputMap)
		{
			ImGui::PushID(&v); // TO Fix

			ImGui::Text("Name: %s", k.c_str());
			ImGui::InputText("Type:", &v.type);
			ImGui::InputText("Shader Binding:", &v.binding);

			ImGui::Separator();
			ImGui::PopID();
		}
		
		if (ImGui::Button("+"))
		{
			ServiceManager::Get<DialogService>()->OpenSingleEntryDialog(nullptr, "Create new Binding", "Enter Unique Name", [&](std::string d) -> void {
				_material->AddInputBinding(d, "Vec2", "value", jsonxx::Object());
			});
		}
		if (ImGui::Button("Save"))
		{
			Save(_path.string());
		}
	}
}