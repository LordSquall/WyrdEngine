/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "ShaderStageVSRes.h"
#include "services/ServiceManager.h"
#include "loaders/ShaderLoader.h"

/* external includes */
#include <core/Application.h>
#include <imgui.h>

namespace Wyrd::Editor
{
	ShaderStageVSRes::ShaderStageVSRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{

	}
	
	Resource::IOResult ShaderStageVSRes::Load(const std::string& filepath)
	{
		Resource::IOResult result = IOResult::Unknown;
		jsonxx::Object o;
		std::ifstream f(filepath);


		// if filepath ends with .raw.vs then we don't want to generate the source and instead just read the file as is
		std::string suffix = ".raw.vs";

		if (filepath.size() >= suffix.size() && filepath.compare(filepath.size() - suffix.size(), suffix.size(), suffix) == 0)
		{
			/* load the shader from file */
			_source.clear();
			_source.append((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		}
		else
		{
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
		}

		return Success;
	}

	Resource::IOResult ShaderStageVSRes::Load(const jsonxx::Object& object)
	{
		/* name */
		std::string name = object.get<jsonxx::String>("name", "default");
		_name = name;

		_Inputs.clear();
		_Outputs.clear();

		/* check for input bindings */
		if (object.has<jsonxx::Array>("inputs"))
		{
			jsonxx::Array inputsArray = object.get<jsonxx::Array>("inputs");

			for (size_t i = 0; i < inputsArray.size(); i++)
			{
				jsonxx::Object inputObject = inputsArray.get<jsonxx::Object>((int)i);

				std::string name = inputObject.get<jsonxx::String>("name");
				std::string type = inputObject.get<jsonxx::String>("type");
				std::string binding = inputObject.get<jsonxx::String>("binding");

				_Inputs[name] = { type, binding };
			}
		}

		/* check for output bindings */
		if (object.has<jsonxx::Array>("outputs"))
		{
			jsonxx::Array outputsArray = object.get<jsonxx::Array>("outputs");

			for (size_t i = 0; i < outputsArray.size(); i++)
			{
				jsonxx::Object outputObject = outputsArray.get<jsonxx::Object>((int)i);

				std::string name = outputObject.get<jsonxx::String>("name");
				std::string type = outputObject.get<jsonxx::String>("type");
				std::string binding = outputObject.get<jsonxx::String>("binding");

				_Outputs[name] = { type, binding };
			}
		}

		Build();
		_isLoaded = true;

		return Success;
	}

	Resource::IOResult ShaderStageVSRes::Save(const std::string& filepath) {
		
		/* construct into a json file */
		jsonxx::Object o;

		/* name */
		o << "name" << _name;

		{
			jsonxx::Array inputArray;
			for (auto& [name, prop] : _Inputs)
			{
				jsonxx::Object inputObj;
				inputObj << "name" << name;
				inputObj << "type" << prop.type;
				inputObj << "binding" << prop.binding;

				inputArray << inputObj;
			}
			o << "inputs" << inputArray;
			
			jsonxx::Array outputArray;
			for (auto& [name, prop] : _Outputs)
			{
				jsonxx::Object outputObj;
				outputObj << "name" << name;
				outputObj << "type" << prop.type;
				outputObj << "binding" << prop.binding;

				outputArray << outputObj;
			}
			o << "outputs" << outputArray;
		}

		std::ofstream out(filepath);
		out << o.json();
		out.close();

		return Success;
	}

	const Icon& ShaderStageVSRes::GetThumbnail()
	{
		return ServiceManager::Get<ResourceService>()->RetrieveIcon("common", "assets_shaderstage_vs");
	}


	bool ShaderStageVSRes::Build()
	{
		/* load the Shader Stage template file */
		std::string rawTemplateContent = Utils::ReadFileToString(Utils::GetEditorResFolder() + "\\templates\\ShaderStage.vs");

		/* populate the template tags */
		std::string populatedTemplateContent = Utils::ReplaceAll(rawTemplateContent, "<<NAME>>", _name.c_str());

		/* build up string block(s) */
		std::ostringstream inputAttributeBlock;
		std::ostringstream inputUniformBlock;
		std::ostringstream outputAttributeBlock;

		int location = 0;
		for (auto& [k, v] : _Inputs)
		{
			if (v.binding == "attribute")
			{
				inputAttributeBlock << "layout(location = " << location << ") in " << v.type << " " << k << ";" << std::endl;
				location++;
			}
			else if (v.binding == "uniform")
			{
				inputUniformBlock << "uniform " << v.type << " " << k << ";" << std::endl;
			}
		}

		for (auto& [k, v] : _Outputs)
		{
			outputAttributeBlock << "out " << v.type << " " << k << ";" << std::endl;
		}

		populatedTemplateContent = Utils::ReplaceAll(populatedTemplateContent, "<<INPUT_ATTRIBUTES>>", inputAttributeBlock.str().c_str());
		populatedTemplateContent = Utils::ReplaceAll(populatedTemplateContent, "<<INPUT_UNIFORMS>>", inputUniformBlock.str().c_str());
		populatedTemplateContent = Utils::ReplaceAll(populatedTemplateContent, "<<OUTPUT_ATTRIBUTES>>", outputAttributeBlock.str().c_str());


		_source = populatedTemplateContent;

		return true;
	}

	const std::string ShaderStageVSRes::GetTypeTag()
	{
		return RESOURCE_TAG_SHADERSTAGE_VS;
	}

	void ShaderStageVSRes::DrawProperties()
	{
		static ImGuiTableFlags ioTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedSame;

		ImGui::Text("FileName: %s", _name.c_str());

		ImGui::SeparatorText("Inputs");

		if (ImGui::BeginTable("inputs_table", 3, ioTableFlags))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Binding");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			for (auto& [k, v] : _Inputs)
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", k.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", v.type.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%s", v.binding.c_str());
				ImGui::TableNextRow();
			}

			ImGui::EndTable();
		}
		
		ImGui::PushID("add_shader_input");
		if (ImGui::Button("+"))
		{
			ServiceManager::Get<DialogService>()->OpenSingleEntryDialog(nullptr, "Create Property", "Enter Unique Name", [&](std::string d) -> void {
				_Inputs[d] = { "Vec3", "attribute" };
				});
		}
		ImGui::PopID();

		ImGui::SeparatorText("Outputs");

		if (ImGui::BeginTable("outputs_table", 3, ioTableFlags))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Binding");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();

			for (auto& [k, v] : _Outputs)
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", k.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", v.type.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%s", v.binding.c_str());
				ImGui::TableNextRow();
			}

			ImGui::EndTable();
		}

		ImGui::PushID("add_shader_output");
		if (ImGui::Button("+"))
		{
			ServiceManager::Get<DialogService>()->OpenSingleEntryDialog(nullptr, "Create Property", "Enter Unique Name", [&](std::string d) -> void {
				_Outputs[d] = { "Vec3", "attribute" };
				});
		}
		ImGui::PopID();

		if (ImGui::Button("Build"))
		{
			Build();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			Save(_path.string());
		}

		ImGui::Text("FileContents");
		ImGui::Text(_source.c_str());
	}
}