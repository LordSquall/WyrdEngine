/* core wyrd includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/MonoUtils.h"
#include "core/scene/Scene.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedCustomObject.h"
#include "core/behaviour/ResourceDescriptors.h"
#include "core/behaviour/PropMonoFactory.h"
#include "core/ecs/ECS.h"
#include "core/ecs/Components.h"
#include "core/ecs/EntitySet.h"

/* external includes */
#include <glm/glm.hpp>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#ifndef MONO_INSTALL_LOC
#define MONO_INSTALL_LOC "C:/PROGRA~1/Mono/"
#endif

#ifndef NATIVE_API_LIB_LOC
#define NATIVE_API_LIB_LOC "C:/Projects/Wyrd/WyrdEngine/lib/Debug/"
#endif


#define WYRD_CORE_API_NAMESPACE "WyrdAPI"
#define WYRD_GAME_API_NAMESPACE "WyrdGame"

namespace Wyrd
{
	Timestep _Timestep;

	Behaviour::Behaviour() :
		_InputMousePos(nullptr),
		_InputMouseButtonState(nullptr),
		_CurrentScene(nullptr),
		_IsRunning(false),
		_RunID(),
		_Scene(nullptr),
		_BehaviourSubsystem(nullptr),
		_PhysicsSubsystem(nullptr),
		_ResourcesSubsystem(nullptr),
		_SceneManager(nullptr),
		_RootDomain(nullptr), 
		_ClientDomain(nullptr), 
		_CoreAssembly(nullptr), 
		_ClientAssembly(nullptr), 
		_CoreImage(nullptr), 
		_ClientImage(nullptr)
	{
		std::string monoLibraryDirectory = MONO_INSTALL_LOC "lib";
		std::string monoExtensionDirectory = MONO_INSTALL_LOC "etc";

		mono_set_dirs(monoLibraryDirectory.c_str(),
			monoExtensionDirectory.c_str());

		/* intiailise the domain */
		_RootDomain = mono_jit_init("Wyrd_Root_Domain");
		if (!_RootDomain)
		{
			WYRD_CORE_ERROR("mono_jit_init failed");
			system("pause");
		}

		/* Build up mapping between Wyrd Key States and managed key state functions */
		_FunctionKeyStateMap[0] = "OnKeyPressed";
		_FunctionKeyStateMap[1] = "OnKeyDown";
		_FunctionKeyStateMap[2] = "OnKeyUp";

		/* Default Input state */
		_Input.MouseButtons[0] = false;
		_Input.MouseButtons[1] = false;
	}

	Behaviour::~Behaviour()
	{
		mono_jit_cleanup((MonoDomain*)_RootDomain);
	}

	void Behaviour::Start(std::shared_ptr<Scene> scene)
	{
		_CurrentScene = scene;

		/* generate a new run ID */
		_RunID = UIDUtils::Create();

		/* store a pointer to the systems */
		_BehaviourSubsystem = Application::Get().GetBehaviourPtr();
		_PhysicsSubsystem = Application::Get().GetPhysicsPtr();
		_ResourcesSubsystem = Application::Get().GetResourcesPtr();

		/* create a managed resource manager proxy object */
		_ResourceManagedProxy = mono_object_new((MonoDomain*)_ClientDomain, *GetClass("ResourceManagerProxy")->ManagedClass);

		/* setup the native pointer to the unmanaged systems */
		if (MonoUtils::SetProperty((MonoImage*)_CoreImage, "WyrdAPI", "ResourceManagerProxy", "NativePtr", _ResourceManagedProxy, { &_BehaviourSubsystem }) == false) { WYRD_ERROR("Failed to set BehaviourManager::NativePtr!"); return; }

		/* setup the interface pointers for all external events */
		if (MonoUtils::SetProperty((MonoImage*)_CoreImage, "WyrdAPI", "SceneManager", "NativePtr", nullptr, { &_SceneManager }) == false)
		{
			WYRD_ERROR("Failed to set SceneManager::NativePtr!");
			return;
		}

		if (_CurrentScene != nullptr)
		{

			/* store the raw scene pointer all ensure it's available at all times within in the VM */
			_Scene = _CurrentScene.get();

			/* link the native scene pointers */
			if (MonoUtils::SetProperty((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "NativePtr", nullptr, { &scene }) == false)
			{
				WYRD_ERROR("Failed to set Scene::NativePtr!");
				return;
			}

			/* populate the texture resources in the VM */
			for (auto& texture : _ResourcesSubsystem->Textures)
			{
				auto textureUID = texture.first.bytes();
				std::vector<void*> args;
				MonoArray* arr = mono_array_new((MonoDomain*)_ClientDomain, mono_get_byte_class(), textureUID.size());

				for (auto i = 0; i < textureUID.size(); i++) {
					mono_array_set(arr, uint8_t, i, textureUID[i]);
				}

				args.push_back(arr);
				args.push_back(&texture.second);

				MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "ResourceManagerProxy", "CreateCachedTextureObject", _ResourceManagedProxy, { args });
			}


			/**
			* We add each of the pools types to the Managed Scene. This will link the Managed classes to the unmanaged pool
			 */
			for (auto& pool : _Scene->componentPools)
			{
				MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "AddPoolRegistration", nullptr,
					{
						mono_string_new((MonoDomain*)_RootDomain, pool->scriptName.c_str()),
						&pool->idx
					});
			}

			/**
			* We register the component pools for the scene. This is important to ensure the component IDs on the native
			* are the same in the VM. This allows simple mapping between the components  in the ECS
			*/
			if (MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "BuildComponentList", nullptr, {}) == false)
			{
				WYRD_ERROR("Failed to BuildComponentList!");
				return;
			}

			/**
			* Create all the entity objects.
			* The key idea here is to allow the VM to create the managed instance using the unmanged entity id
			* We can then store the MonoObject to enable further actions
			*/
			MonoMethod* createEntityMethod = MonoUtils::FindMethodInClass((MonoImage*)_CoreImage, WYRD_CORE_API_NAMESPACE, "EntityManager", "CreateEntity", 1);
			for (auto& e : _CurrentScene->entities)
			{
				MonoObject* exc = nullptr;
				void* createEntityArgs[1] = { &e };
				MonoObject* newObject = mono_runtime_invoke(createEntityMethod, nullptr, &createEntityArgs[0], &exc);
				ScriptedEntities.insert({ e.id, newObject });

				if (exc != nullptr) mono_print_unhandled_exception(exc);
			}

			/**
			* Create a managed object for each of the script components.
			*/
			for (Entity e : EntitySet<ScriptComponent>(*_CurrentScene.get()))
			{
				ScriptComponent* scriptComponent = _CurrentScene->Get<ScriptComponent>(e);

				/* retrieve script class */
				auto scriptClass = GetCustomClassByUID(scriptComponent->scriptId);

				if (scriptClass != nullptr)
				{
					/* create the actual object*/
					auto scriptObject = std::make_shared<ScriptedCustomObject>((MonoDomain*)_ClientDomain, _ClientImage, GetCustomClassByUID(scriptComponent->scriptId).get(), _ScriptedClasses["ScriptedEntity"].get(), e);

					/* add to the custom object list */
					_ECSScriptedCustomObjects[scriptComponent->scriptId].push_back(scriptObject);

					/* map the instance Id to the location in the map vector */
					scriptComponent->instanceId = (uint32_t)_ECSScriptedCustomObjects[scriptComponent->scriptId].size() - 1;

					ScriptedCustomObject* obj = GetCustomObject(scriptComponent->scriptId, scriptComponent->instanceId);

					for (auto& prop : *scriptComponent->properties)
					{
						PropMonoFactory::SetProperty(prop.second.get(), (MonoImage*)_ClientImage, scriptClass->GetName(), obj->Object);
					}
				}
				else
				{
					scriptComponent->instanceId = -1;
				}
			}

			/**
			* Call the initial start function on each of the managed objects
			*/
			for (Entity e : EntitySet<ScriptComponent>(*_CurrentScene.get()))
			{
				ScriptComponent* scriptComponent = _CurrentScene->Get<ScriptComponent>(e);

				if (scriptComponent->instanceId != -1)
				{
					ScriptedCustomObject* obj = GetCustomObject(scriptComponent->scriptId, scriptComponent->instanceId);

					if (MonoUtils::InvokeMethod((MonoImage*)_ClientImage, "WyrdGame", obj->TypeName, "OnStart", obj->Object, { }) == false)
					{
						WYRD_ERROR("Unable to invoke Key Event!");
					}
				}
			}

			_IsRunning = true;
		}
	}

	void Behaviour::Stop()
	{
		/* Call the managed scene to reset any state */
		if (MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "Reset", nullptr, {}) == false)
		{
			WYRD_ERROR("Failed to Reset Managed Scene!");
			return;
		}

		_IsRunning = false;
	}

	void Behaviour::Restart(std::shared_ptr<Scene> scene)
	{
		/* Stop the behaviour model */
		Stop();

		/* Start the behaviour model */
		Start(scene);
	}

	void Behaviour::Update(Timestep ts)
	{
		/* we only action anything in the function if we are in running mode */
		if (_IsRunning == true)
		{
			std::vector<void*> updateArgs = std::vector<void*>({ &ts });

			for (Entity e : EntitySet<ScriptComponent>(*_CurrentScene.get()))
			{
				ScriptComponent* scriptComponent = _CurrentScene->Get<ScriptComponent>(e);
			
				if (scriptComponent->instanceId != -1)
				{
					MonoObject* ecx = nullptr;
					ScriptedCustomObject* obj = GetCustomObject(scriptComponent->scriptId, scriptComponent->instanceId);
					MonoMethod* m = &*obj->GetMethod("OnUpdate");
					MonoObject* o = &*obj->Object;
					mono_runtime_invoke(m, o, &updateArgs[0], &ecx);
				
					if (ecx != nullptr)
						mono_print_unhandled_exception(ecx);
				}
			}
		}
	}

	ScriptedCustomObject* Behaviour::GetCustomObject(UID& uid, uint32_t instanceID)
	{
		return _ECSScriptedCustomObjects[uid][instanceID].get();
	}

	void Behaviour::SetInputState(int key, int state)
	{
		UID currentRunID = _RunID;

		if (_IsRunning == true)
		{
			if (_CurrentScene != nullptr)
			{
				for (Entity e : EntitySet<ScriptComponent>(*_CurrentScene.get()))
				{
					ScriptComponent* scriptComponent = _CurrentScene->Get<ScriptComponent>(e);
				
					if (scriptComponent->instanceId != -1)
					{
						ScriptedCustomObject* obj = GetCustomObject(scriptComponent->scriptId, scriptComponent->instanceId);
						MonoObject* o = &*obj->Object;
				
						if (MonoUtils::InvokeMethod((MonoImage*)_ClientImage, "WyrdGame", obj->TypeName, _FunctionKeyStateMap[state], o, { &key }) == false)
						{
							WYRD_ERROR("Unable to invoke Key Event!");
						}
				
						if (currentRunID != _RunID)
						{
							return;
						}
					}
				}
			}
		}
	}

	void Behaviour::SetMouseState(float xPos, float yPos)
	{
		_Input.MouseInput.x = xPos;
		_Input.MouseInput.y = yPos;
	}

	void Behaviour::SetMouseButtonState(int i, bool state)
	{
		_Input.MouseButtons[i] = state;
	}
	
	void Behaviour::UpdateInputState()
	{
		std::vector<void*> args;
		args.push_back(&_Input.MouseInput.x);
		args.push_back(&_Input.MouseInput.y);
		args.push_back(&_Input.MouseButtons[0]);
		args.push_back(&_Input.MouseButtons[1]);

		MonoObject* exception = nullptr;
		auto properties = _ScriptedClasses["Vector2"]->Properties;
		//mono_runtime_invoke((MonoMethod*)(*properties)["X"]->GetSetter(), (MonoObject*)_InputMousePos, &args[0], &exception);
		//if (exception != nullptr) mono_print_unhandled_exception(exception);
		//
		//mono_runtime_invoke((MonoMethod*)(*properties)["Y"]->GetSetter(), (MonoObject*)_InputMousePos, &args[1], &exception);
		//if (exception != nullptr) mono_print_unhandled_exception(exception);

		mono_array_set((MonoArray*)_InputMouseButtonState, bool, 0, _Input.MouseButtons[0]);
		mono_array_set((MonoArray*)_InputMouseButtonState, bool, 1, _Input.MouseButtons[1]);
	}

	void* Behaviour::GetDomain()
	{
		return _ClientDomain;
	}

	std::shared_ptr<ScriptedClass> Behaviour::GetClass(std::string name) 
	{ 
		return _ScriptedClasses[name]; 
	}

	std::shared_ptr<ScriptedClass> Behaviour::GetCustomClass(std::string name) 
	{ 
		return _ScriptedCustomClasses[name]; 
	}

	std::shared_ptr<ScriptedClass> Behaviour::GetCustomClassByUID(const UID& uid)
	{
		for (auto& sc : _ScriptedCustomClasses)
		{
			if (sc.second->GetUID() == uid)
				return sc.second;
		}

		return nullptr;
	}

	MonoObject* Behaviour::RetrieveCachedManagedTextureObject(const Wyrd::UID& uid)
	{
		std::vector<void*> args;
		MonoArray* arr = mono_array_new((MonoDomain*)_ClientDomain, mono_get_byte_class(), uid.bytes().size());
		for (auto i = 0; i < uid.bytes().size(); i++) {
			mono_array_set(arr, uint8_t, i, uid.bytes()[i]);
		}
		args.push_back(arr);

		MonoClass* clsPtr = mono_class_from_name((MonoImage*)_CoreImage, WYRD_CORE_API_NAMESPACE, "ResourceManagerProxy");
		if (clsPtr == nullptr)
		{
			return false;
		}

		MonoMethod* methodPtr = mono_class_get_method_from_name(clsPtr, "RetrieveCachedTextureObject", 1);
		if (methodPtr == nullptr)
		{
			return nullptr;
		}

		MonoObject* exc = nullptr;
		MonoObject* foundObject = mono_runtime_invoke(methodPtr, _ResourceManagedProxy, (args.size() == 0) ? nullptr : &args[0], &exc);
		if (exc != nullptr)
		{
			mono_print_unhandled_exception(exc);
			return nullptr;
		}

		return foundObject;
	}

	void Behaviour::LoadBehaviourModel(const std::vector<std::filesystem::path>& files, const std::filesystem::path& inputFile)
	{
		std::string apiLibraryLocation = NATIVE_API_LIB_LOC "WyrdAPI/WyrdAPI.dll";

		/* Read the core library into memory */
		std::ifstream coreLibFileStream(apiLibraryLocation.c_str(), std::ios::binary);
		std::vector<char> coreLibData((std::istreambuf_iterator<char>(coreLibFileStream)), (std::istreambuf_iterator<char>()));
		coreLibFileStream.close();

		/* Read the client library into memory */
		std::ifstream clientLibFileStream(inputFile.c_str(), std::ios::binary);
		std::vector<char> clientLibData((std::istreambuf_iterator<char>(clientLibFileStream)), (std::istreambuf_iterator<char>()));
		clientLibFileStream.close();

		LoadBehaviourModel(files, coreLibData, clientLibData);
	}

	void Behaviour::LoadBehaviourModel(const std::vector<std::filesystem::path>& files, std::vector<char>& coreData, std::vector<char>& clientData)
	{
		/* Unload the client domain */
		if (_ClientDomain != nullptr)
		{
			mono_domain_set((MonoDomain*)_RootDomain, true);
			mono_domain_unload((MonoDomain*)_ClientDomain);
		}

		/* Create a new app domain */
		_ClientDomain = mono_domain_create_appdomain("Wyrd_Client_Domain", nullptr);

		/* Switch to the client domain */
		mono_domain_set((MonoDomain*)_ClientDomain, true);

		LoadAssembly(_ClientDomain, &_CoreImage, &_CoreAssembly, "WyrdAPI", coreData);
		LoadAssembly(_ClientDomain, &_ClientImage, &_ClientAssembly, "WyrdGame", clientData);

		/* Search for all valid classes in the DLL and add build the scripted classes */
		const MonoTableInfo* table_info = mono_image_get_table_info((MonoImage*)_CoreImage, MONO_TABLE_TYPEDEF);
		for (int i = 0; i < mono_table_info_get_rows(table_info); i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap((MonoImage*)_CoreImage, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap((MonoImage*)_CoreImage, cols[MONO_TYPEDEF_NAMESPACE]);

			/** There will mostly be a large number of classes exported due to dependancies,
				However we only care about the WyrdAPI.* namespace
			*/
			if (strcmp(nameSpace, "WyrdAPI") == 0)
			{
				/* Add class to the managed class map */
				_ManagedClasses[name] = mono_class_from_name((MonoImage*)_CoreImage, "WyrdAPI", name);

				/* Create a new Scripted Class */
				std::shared_ptr<ScriptedClass> newScriptedClass = std::make_shared<ScriptedClass>(name, &_ManagedClasses[name], _ClientDomain);

				/* Store within the Behaviour Subsystem */
				_ScriptedClasses[name] = newScriptedClass;
			}
		}

		/* Create each of the new scripted classes */
		for (auto& file : files)
		{
			// Get the file name
			std::string className = file.stem().string();

			/* We want to be sure to exclude the assembly info class */
			if (className != "AssemblyInfo")
			{
				/* retrieve the managed class */
				MonoClass* managedClass = mono_class_from_name((MonoImage*)_ClientImage, "WyrdGame", className.c_str());

				/* Add class to the managed class map */
				_ManagedClasses[className] = mono_class_from_name((MonoImage*)_ClientImage, "WyrdGame", className.c_str());

				std::shared_ptr<ScriptedClass> newScript = std::make_shared<ScriptedClass>(className, &_ManagedClasses[className], _ClientDomain);

				_ScriptedCustomClasses[className] = newScript;
			}
		}

		/* Retrieve fixed objects */
		auto inputProps = _ScriptedClasses["Input"]->Properties;
		//_InputMousePos = mono_runtime_invoke((MonoMethod*)(*inputProps)["MousePos"].GetGetter(), nullptr, nullptr, nullptr);
		//_InputMouseButtonState = mono_runtime_invoke((MonoMethod*)(*inputProps)["MouseButtons"]->GetGetter(), nullptr, nullptr, nullptr);
	}

	void Behaviour::LoadAssembly(void* domain, void** image, void** assembly, const std::string& ns, std::vector<char>& assemblyData)
	{		
		/* need to add the WyrdAPI dll to the assembly to allow runtime access */
		MonoImageOpenStatus status = MONO_IMAGE_ERROR_ERRNO;
		
		
		(*image) = mono_image_open_from_data(&assemblyData[0], (uint32_t)assemblyData.size(), 1, &status);
		if (status != MONO_IMAGE_OK || (*image) == NULL)
		{
			return;
		}
		
		(*assembly) = mono_assembly_load_from((MonoImage*)(*image), "", &status);
		if (status != MONO_IMAGE_OK || (*assembly) == NULL) {
			return; 
		} 
		
		/* Search for all valid classes in the DLL and add build the scripted classes */
		const MonoTableInfo* table_info = mono_image_get_table_info((MonoImage*)(*image), MONO_TABLE_TYPEDEF);
		for (int i = 0; i < mono_table_info_get_rows(table_info); i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap((MonoImage*)(*image), cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap((MonoImage*)(*image), cols[MONO_TYPEDEF_NAMESPACE]);
			
			/** There will mostly be a large number of classes exported due to dependancies, 
			However we only care about the WyrdAPI.* namespace*/
			if (strcmp(nameSpace, ns.c_str()) == 0)
			{
				MonoClass* monoClass;
				monoClass = mono_class_from_name((MonoImage*)(*image), ns.c_str(), name);

				if (monoClass != nullptr)
				{
					WYRD_CORE_TRACE("\tClasses Found: {0}::{1}", ns, name);
				}
			}
		}
	}

	void Behaviour::DebugPrintFunc(const std::string& s)
	{
		WYRD_TRACE(s);
	}
}