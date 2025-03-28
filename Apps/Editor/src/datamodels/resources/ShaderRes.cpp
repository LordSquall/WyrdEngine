/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Shader.h>

/* local includes */
#include "ShaderRes.h"
#include "ShaderStageRes.h"
#include "services/ServiceManager.h"
#include "loaders/ShaderLoader.h"
#include "support/ImGuiUtils.h"
#include "serial/TypeSerialisers.h"
#include "serial/ComponentSerialiserFactory.h"
#include "serial/CustomComponentSerialisationFunctions.h"

/* external includes */
#include <core/Application.h>
#include <core/Log.h>

namespace Wyrd::Editor
{
	ShaderRes::ShaderRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{
		_onSuccessfulLoadCallback = [=]() {
			Build(true);
		};
	}
	
	Resource::IOResult ShaderRes::Load(const std::string& filepath)
	{		
		/* shaders should always be loaded on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>()->Publish(Events::EventType::LoadAsset, std::make_unique<Events::LoadAssetArgs>(this));
		}
		else
		{
			std::string name;
			std::string vs;
			std::string fs;

			Resource::IOResult result = FileNotFound;
			jsonxx::Object o;
			std::ifstream f(filepath);

			if (f.is_open() == true) {
				std::ostringstream ss;
				ss << f.rdbuf();

				if (o.parse(ss.str()) == true)
				{
					_path = filepath;

					result = Load(o);

					if (result == Success && _onSuccessfulLoadCallback != nullptr)
					{
						_onSuccessfulLoadCallback();
					}
				}
				else
				{
					result = FileMalformed;
				}
			}

			if (result != Success && _onFailureLoadCallback != nullptr)
			{
				_onFailureLoadCallback();
			}

			return result;
		}

		return Success;
	}

	Resource::IOResult ShaderRes::Load(const jsonxx::Object& obj)
	{	
		/* name */
		_name = obj.get<jsonxx::String>("name", "default");

		/* shader stage resource id's */
		if (obj.has<jsonxx::String>("vertexStageID") && obj.has<jsonxx::String>("fragmentStageID"))
		{
			_VertexShaderStageId = UID(obj.get<jsonxx::String>("vertexStageID", "default"));
			_FragmentShaderStageId = UID(obj.get<jsonxx::String>("fragmentStageID", "default"));
		}

		/* build shader description */
		ShaderDesc shaderDesc;
		shaderDesc.resource.guid = UIDUtils::Create();
		shaderDesc.resource.name = _name;

		/* create the core renderer resource and load */
		if (_shader == nullptr)
			_shader.reset(Shader::Create(shaderDesc));

		Application::Get().GetResources().Shaders[_resourceID] = _shader;
		
		return Success;
	}

	Resource::IOResult ShaderRes::Save(const std::string& filepath) {
		
		/* construct into a json file */
		jsonxx::Object o;

		/* name */
		o << "name" << _name;

		/* vertex stage id */
		if (_VertexStage != nullptr)
		{
			o << "vertexStageID" << _VertexStage->GetResourceID();
		}
		else
		{
			o << "vertexStageID" << "unbound";
		}

		/* fragment stage id */
		if (_FragmentStage != nullptr)
		{
			o << "fragmentStageID" << _FragmentStage->GetResourceID();
		}
		else
		{
			o << "fragmentStageID" << "unbound";
		}

		std::ofstream out(filepath);
		out << o.json();
		out.close();

		return Success;
	}

	const Icon& ShaderRes::GetThumbnail()
	{
		return ServiceManager::Get<ResourceService>()->RetrieveIcon("common", "assets_shader");
	}

	const std::string ShaderRes::GetTypeTag()
	{
		return RESOURCE_TAG_SHADER;
	}

	void ShaderRes::ResolveReferences()
	{
		std::shared_ptr<ResourceService> resourceService = ServiceManager::Get<ResourceService>();

		/* Resolve Resources */
		_VertexStage = resourceService->GetResourceByID<ShaderStageRes>(_VertexShaderStageId);
		_VertexStage->ResolveReferences();
		_FragmentStage = resourceService->GetResourceByID<ShaderStageRes>(_FragmentShaderStageId);
		_FragmentStage->ResolveReferences();

		/* Hook up 'upward' resource chains */
		_VertexStage->AddListeningResource(this->shared_from_this());
		_FragmentStage->AddListeningResource(this->shared_from_this());

		/* apply any resources */
		/* N/A */
	}

	void ShaderRes::Build(bool isRebuild)
	{
		if (_VertexStage != nullptr && _FragmentStage != nullptr)
			_isLoaded = _shader->Build(_VertexStage->GetSource(), _FragmentStage->GetSource(), isRebuild);
	}

	void ShaderRes::DrawProperties()
	{
		ImGui::Text("Shader Name: %s", _name.c_str());

		if (_VertexStage != nullptr)
		{
			ImGui::Text("Vertex Stage: %s", _VertexStage->GetName().c_str());
		}
		else
		{
			ImGui::Text("Vertex Stage: <unbound>");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(RESOURCE_TAG_SHADERSTAGE))
			{
				std::shared_ptr<ResourceService> resourceService = ServiceManager::Get<ResourceService>();

				UID* shaderStageID = (UID*)payload->Data;
				std::shared_ptr<ShaderStageRes> res = resourceService->GetResourceByID<ShaderStageRes>(*shaderStageID);
				if (res != nullptr)
				{
					_VertexStage = res;
					Save(_path.string());
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (_FragmentStage != nullptr)
		{
			ImGui::Text("Fragment Stage: %s", _FragmentStage->GetName().c_str());
		}
		else
		{
			ImGui::Text("Fragment Stage: <unbound>");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(RESOURCE_TAG_SHADERSTAGE))
			{
				std::shared_ptr<ResourceService> resourceService = ServiceManager::Get<ResourceService>();

				UID* shaderStageID = (UID*)payload->Data;
				std::shared_ptr<ShaderStageRes> res = resourceService->GetResourceByID<ShaderStageRes>(*shaderStageID);
				if (res != nullptr)
				{
					_FragmentStage = res;
					Save(_path.string());
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}
