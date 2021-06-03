#pragma once

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

namespace Wyrd
{
	Timestep _Timestep;

	Behaviour::Behaviour() : _IsRunning(false), _RootDomain(nullptr), _ClientDomain(nullptr), _CoreAssembly(nullptr), _ClientAssembly(nullptr), _CoreImage(nullptr), _ClientImage(nullptr)
	{
		std::string monoLibraryDirectory = MONO_INSTALL_LOC "lib";
		std::string monoExtensionDirectory = MONO_INSTALL_LOC "etc";

		mono_set_dirs(monoLibraryDirectory.c_str(),
			monoExtensionDirectory.c_str());

		/* intiailise the domain */
		_RootDomain = mono_jit_init("Wyrd_Root_Domain");
		if (!_RootDomain)
		{
			std::cout << "mono_jit_init failed" << std::endl;
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
		_IsRunning = true;

		/* store a pointer to the systems */
		_BehaviourSubsystem = Application::Get().GetBehaviourPtr();
		_PhysicsSubsystem = Application::Get().GetPhysicsPtr();
		_ResourcesSubsystem = Application::Get().GetResourcesPtr();

		/* pass in the subsystem pointers to the managed domain */
		MonoClass* monoClass;
		monoClass = mono_class_from_name((MonoImage*)_CoreImage, "WyrdAPI", "SubsystemManager");

		if (_CurrentScene != nullptr)
		{
			_Scene = _CurrentScene.get();

			if (MonoUtils::SetProperty((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "NativePtr", nullptr, { &scene }) == false)
			{
				WYRD_ERROR("Failed to set Scene::NativePtr!");
				return;
			}

			for (auto& pool : _Scene->componentPools)
			{
				WYRD_TRACE(pool->scriptName);
				MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "AddPoolRegistration", nullptr, 
					{ 
						mono_string_new((MonoDomain*)_RootDomain, pool->scriptName.c_str()), 
						&pool->idx 
					});
			}


			if (MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "BuildComponentList", nullptr, {}) == false)
			{
				WYRD_ERROR("Failed to BuildComponentList!");
				return;
			}

			for (auto& e : _CurrentScene->entities)
			{
				/* create a new entity object in mono */
				MonoMethod* createEntityMethod = _ScriptedClasses["EntityManager"]->Methods["CreateEntity"]->GetManagedMethod();

				MonoObject* exc = nullptr;
				void* createEntityArgs[1] = { &e };
				mono_runtime_invoke(createEntityMethod, nullptr, &createEntityArgs[0], &exc);

				if (exc != nullptr) mono_print_unhandled_exception(exc);
			}

			/* create a managed object for each of the script components */
			for (Entity e : EntitySet<ECSScriptComponent, ECSScriptInternalComponent>(*_CurrentScene.get()))
			{
				ECSScriptComponent* scriptComponent = _CurrentScene->Get<ECSScriptComponent>(e);
				ECSScriptInternalComponent* scriptComponentInternal = _CurrentScene->Get<ECSScriptInternalComponent>(e);

				/* create the actual object*/
				_ECSScriptedCustomObjects[scriptComponent->script].push_back(std::make_shared<ScriptedCustomObject>((MonoDomain*)_ClientDomain, _ClientImage, GetCustomClassByUID(scriptComponent->script).get(), _ScriptedClasses["ScriptedEntity"].get(), e));
				
				/* map the instance Id to the location in the map vector */
				scriptComponentInternal->instanceID = (uint32_t)_ECSScriptedCustomObjects[scriptComponent->script].size() - 1;
			}

			/* call the initial start function on each of the managed objects */
			for (Entity e : EntitySet<ECSScriptComponent, ECSScriptInternalComponent>(*_CurrentScene.get()))
			{
				ECSScriptComponent* scriptComponent = _CurrentScene->Get<ECSScriptComponent>(e);
				ECSScriptInternalComponent* scriptComponentInternal = _CurrentScene->Get<ECSScriptInternalComponent>(e);

				ScriptedCustomObject* obj = GetCustomObject(scriptComponent->script, scriptComponentInternal->instanceID);
				MonoMethod* m = &*obj->GetMethod("OnStart");
				MonoObject* o = &*obj->Object;
				mono_runtime_invoke(m, o, nullptr, nullptr);
			}
		}
	}

	void Behaviour::Stop()
	{
		/* clear managed objects */
		_ScriptedCustomObjects.clear();

		if (MonoUtils::InvokeMethod((MonoImage*)_CoreImage, "WyrdAPI", "Scene", "Reset", nullptr, {}) == false)
		{
			WYRD_ERROR("Failed to Reset Managed Scene!");
			return;
		}

		_IsRunning = false;
	}

	void Behaviour::Update(Timestep ts)
	{
		/* we only action anything in the function if we are in running mode */
		if (_IsRunning == true)
		{
			std::vector<void*> updateArgs = std::vector<void*>({ &ts });

			for (Entity e : EntitySet<ECSScriptComponent, ECSScriptInternalComponent>(*_CurrentScene.get()))
			{
				ECSScriptComponent* scriptComponent = _CurrentScene->Get<ECSScriptComponent>(e);
				ECSScriptInternalComponent* scriptComponentInternal = _CurrentScene->Get<ECSScriptInternalComponent>(e);

				// TODO rework mapping
				MonoObject* ecx = nullptr;
				ScriptedCustomObject* obj = GetCustomObject(scriptComponent->script, scriptComponentInternal->instanceID);
				MonoMethod* m = &*obj->GetMethod("OnUpdate");
				MonoObject* o = &*obj->Object;
				mono_runtime_invoke(m, o, &updateArgs[0], &ecx);

				if (ecx != nullptr)
					mono_print_unhandled_exception(ecx);
			}
		}
	}


	ScriptedCustomObject* Behaviour::GetCustomObject(UID& uid, uint32_t instanceID)
	{
		return _ECSScriptedCustomObjects[uid][instanceID].get();
	}

	void Behaviour::SetInputState(int key, int state)
	{
		if (_IsRunning == true)
		{
			if (_CurrentScene != nullptr)
			{
				for (Entity e : EntitySet<ECSScriptComponent, ECSScriptInternalComponent>(*_CurrentScene.get()))
				{
					ECSScriptComponent* scriptComponent = _CurrentScene->Get<ECSScriptComponent>(e);
					ECSScriptInternalComponent* scriptComponentInternal = _CurrentScene->Get<ECSScriptInternalComponent>(e);

					ScriptedCustomObject* obj = GetCustomObject(scriptComponent->script, scriptComponentInternal->instanceID);
					MonoObject* o = &*obj->Object;

					if (MonoUtils::InvokeMethod((MonoImage*)_ClientImage, "WyrdGame", obj->TypeName, _FunctionKeyStateMap[state], o, { &key }) == false)
					{
						WYRD_ERROR("Unable to invoke Key Event!");
					} 
				}
			}
		}
	}

	void Behaviour::SetMouseState(float xPos, float yPos)
	{
		_Input.MouseInput = { xPos, yPos };
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
		mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Vector2"]->Properties["X"]->GetSetter(), (MonoObject*)_InputMousePos, &args[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Vector2"]->Properties["Y"]->GetSetter(), (MonoObject*)_InputMousePos, &args[1], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

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

	std::shared_ptr<ScriptedClass> Behaviour::GetCustomClassByUID(UID& uid)
	{
		for (auto& sc : _ScriptedCustomClasses)
		{
			if (sc.second->GetUID() == uid)
				return sc.second;
		}

		return nullptr;
	}

	std::shared_ptr<ScriptedCustomObject> Behaviour::GetCustomObject(UID uid)
	{ 
		return _ScriptedCustomObjects[uid]; 
	}

	void Behaviour::AddScriptedCustomObject(UID uid, std::shared_ptr<ScriptedCustomObject> customObject)
	{
		_ScriptedCustomObjects[uid] = customObject;
	}

	void Behaviour::CompileAll(const std::vector<std::string>& files, const std::string& outputFile, CompileResults& results)
	{
		// mono compiler script command
		std::string command = "mcs ";

		// add all the files in the project
		for (auto& file : files)
		{
			command += file + " ";
		}

		std::string file_name = "error_output.txt";

		// delete the error file 
		std::remove(file_name.c_str());

		// add library and wyrd runtime 
		command += " -target:library -lib:" MONO_INSTALL_LOC  "lib/mono/4.5/Facades/," NATIVE_API_LIB_LOC "WyrdAPI/ -r:System.Runtime.InteropServices.dll,WyrdAPI.dll -debug ";

		// set the putput file
		command += "-out:" + outputFile;

		// run the command to compile
		std::system((command + " 2> " + file_name).c_str()); // redirect output to file

		// get the size of the file to determine if there were any errors or warnings
		if (std::filesystem::file_size(file_name) > 0)
		{
			// read all the lines into a vector
			std::ifstream file(file_name);
			std::string str;
			std::vector<std::string> messages;

			while (std::getline(file, str))
			{
				// Line contains string of length > 0 then save it in vector
				if (str.size() > 0)
					messages.push_back(str);
			}
			
			// Mark the compiles as unsuccessful
			results.success = false;
			results.errors = messages;

			return;
		}
	}

	void Behaviour::LoadBehaviourModel(const std::vector<std::string>& files, const std::string& inputFile)
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

	void Behaviour::LoadBehaviourModel(const std::vector<std::string>& files, std::vector<char>& coreData, std::vector<char>& clientData)
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

		//std::string apiLibraryLocation = NATIVE_API_LIB_LOC "WyrdAPI/WyrdAPI.dll";
		//std::string assemblyPath = inputFile.c_str();

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
			std::filesystem::path filename = file;
			std::string className = filename.stem().string();

			/* retrieve the managed class */
			MonoClass* managedClass = mono_class_from_name((MonoImage*)_ClientImage, "WyrdGame", className.c_str());

			/* Add class to the managed class map */
			_ManagedClasses[className] = mono_class_from_name((MonoImage*)_ClientImage, "WyrdGame", className.c_str());

			std::shared_ptr<ScriptedClass> newScript = std::make_shared<ScriptedClass>(className, &_ManagedClasses[className], _ClientDomain);

			_ScriptedCustomClasses[className] = newScript;
		}

		/* Retrieve fixed objects */
		_InputMousePos = mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Input"]->Properties["MousePos"]->GetGetter(), nullptr, nullptr, nullptr);
		_InputMouseButtonState = mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Input"]->Properties["MouseButtons"]->GetGetter(), nullptr, nullptr, nullptr);
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
					//WYRD_CORE_TRACE("\tClasses Found: {0}::{1}", ns, name);
				}
			}
		}
	}

	void Behaviour::DebugPrintFunc(const std::string& s)
	{
		WYRD_TRACE(s);
	}
}