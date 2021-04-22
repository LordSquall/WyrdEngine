#pragma once

/* core osiris includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/Behaviour.h"
#include "core/behaviour/MonoUtils.h"
#include "core/scene/Scene.h"
#include "core/scene/Layer2D.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/SpriteComponent.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/renderer/Texture.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/ScriptedCustomObject.h"

/* external includes */
#include <glm/glm.hpp>
#include <iostream>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#ifndef MONO_INSTALL_LOC
#define MONO_INSTALL_LOC "C:/PROGRA~1/Mono/"
#endif

namespace Osiris
{
	Timestep _Timestep;

	Behaviour::Behaviour() : _IsRunning(false), _RootDomain(nullptr), _ClientDomain(nullptr), _CoreAssembly(nullptr), _ClientAssembly(nullptr), _CoreImage(nullptr), _ClientImage(nullptr)
	{
		std::string monoLibraryDirectory = MONO_INSTALL_LOC "lib";
		std::string monoExtensionDirectory = MONO_INSTALL_LOC "etc";

		mono_set_dirs(monoLibraryDirectory.c_str(),
			monoExtensionDirectory.c_str());

		/* intiailise the domain */
		_RootDomain = mono_jit_init("Osiris_Root_Domain");
		if (!_RootDomain)
		{
			std::cout << "mono_jit_init failed" << std::endl;
			system("pause");
		}

		/* Build up mapping between Osiris Key States and managed key state functions */
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
		monoClass = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", "SubsystemManager");

		MonoProperty* behaviourProp = mono_class_get_property_from_name(monoClass, "Behaviour");
		MonoProperty* physicsProp = mono_class_get_property_from_name(monoClass, "Physics");
		MonoProperty* resourcesProp = mono_class_get_property_from_name(monoClass, "Resources");

		std::vector<void*> behaviourArgs = std::vector<void*>({ &_BehaviourSubsystem });
		mono_property_set_value(behaviourProp, nullptr, &behaviourArgs[0], nullptr);

		std::vector<void*> physicsArgs = std::vector<void*>({ &_PhysicsSubsystem });
		mono_property_set_value(physicsProp, nullptr, &physicsArgs[0], nullptr);

		std::vector<void*> resourceArgs = std::vector<void*>({ &_ResourcesSubsystem });
		mono_property_set_value(resourcesProp, nullptr, &resourceArgs[0], nullptr);

		/* add all the managed texture objects */
		for (auto& textureResource : _ResourcesSubsystem->Textures)
		{
			_NativePtrMap.push_back(textureResource.second.get());
			MonoObject* newResource = MonoUtils::CreateNewUnmanagedObject((MonoDomain*)_ClientDomain, _ScriptedClasses["Texture"], this, &_NativePtrMap.back());

			std::vector<void*> args;
			args.push_back(newResource);

			MonoObject* exception = nullptr;
			mono_runtime_invoke(_ScriptedClasses["ResourceManager"]->Methods["AddResource"]->GetManagedMethod(), nullptr, &args[0], &exception);
			if (exception != nullptr) mono_print_unhandled_exception(exception);
		}

		/* first stage is to create a instance of each gameobject within mono */
		if (_CurrentScene != nullptr)
		{
			BuildManagedGameObjects();

			/* next stage to build hierarchy in between GameObjects */
			//BuildManagedGameObjectHierarchy();

			/* next stage is to pass over any script components and create custom objects for each one */
			LinkManagedGameObjects();

			/* next stage is to pass over any script components and link properties */
			LinkGameObjectProperties();

			/* final stage, run the On start lifecycle functions */
			StartManagedGameObjects();
		}
	}

	void Behaviour::Stop()
	{
		/* clear managed objects */
		_ScriptedGameObjects.clear();
		_ScriptedCustomObjects.clear();

		_NativePtrMap.clear();

		mono_runtime_invoke(MonoUtils::FindMethodInClass(GetClass("GameObjectManager"), "Reset", 0, true), NULL, NULL, NULL);

		_IsRunning = false;
	}

	void Behaviour::Update(Timestep ts)
	{
		/* we only action anything in the function if we are in running mode */
		if (_IsRunning == true)
		{
			UpdateInputState();

			/* Update the 'timestep' object. This is bound to the lua state */
			_Timestep = ts;

			if (_CurrentScene != nullptr)
			{
				UpdateManagedGameObjects(_Timestep);
			}
		}
	}

	void Behaviour::SetInputState(int key, int state)
	{
		if (_IsRunning == true)
		{
			if (_CurrentScene != nullptr)
			{
				/* traverse each of the gameobjects within the scene*/
				for (auto& sl : _CurrentScene->GetLayers())
				{
					for (auto& go : sl->GetGameObjects())
					{
						SetInputState(go.get(), key, state);
					}
				}
			}
		}
	}

	void Behaviour::SetInputState(GameObject* gameObject, int key, int state)
	{
		for (auto& component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				/* convert to script component type */
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned object */
				if (scriptComponent->GetCustomObject() != nullptr)
				{
					/* build arg list for the key event functions */
					std::vector<void*> args = std::vector<void*>({ &key });

					/* call custom script object key event function */
					MonoUtils::ExecuteScriptMethod(scriptComponent, _FunctionKeyStateMap[state], args);
				}
			}
		}

		for (auto& go : gameObject->GetGameObjects())
		{
			SetInputState(go.get(), key, state);
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

	std::shared_ptr<ScriptedGameObject> Behaviour::GetGameObject(UID uid)
	{ 
		return _ScriptedGameObjects[uid]; 
	}

	std::shared_ptr<ScriptedCustomObject> Behaviour::GetCustomObject(UID uid)
	{ 
		return _ScriptedCustomObjects[uid]; 
	}

	void Behaviour::AddScriptedGameObject(UID uid, GameObject* gameObject, void* managedObject)
	{
		_ScriptedGameObjects[uid] = std::make_shared<ScriptedGameObject>(this, _ScriptedClasses["GameObject"], gameObject, managedObject);
	}

	void Behaviour::AddScriptedCustomObject(UID uid, std::shared_ptr<ScriptedCustomObject> customObject)
	{
		_ScriptedCustomObjects[uid] = customObject;
	}

	void Behaviour::BroadcastTriggerCall(std::shared_ptr<GameObject> gameObject, std::string& funcName, std::shared_ptr<GameObject> triggerObject, std::vector<void*> args)
	{
		for(auto &component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				args.insert(args.begin(), _ScriptedGameObjects[triggerObject->uid]->Object);

				MonoUtils::ExecuteScriptMethod((ScriptComponent*)&*component, funcName, args);
			}
		}
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

		// add library and osiris runtime 
		command += " -target:library -lib:" MONO_INSTALL_LOC  "lib/mono/4.5/Facades/," NATIVE_API_LIB_LOC "OsirisAPI/ -r:System.Runtime.InteropServices.dll,OsirisAPI.dll -debug ";

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
		std::string apiLibraryLocation = NATIVE_API_LIB_LOC "OsirisAPI/OsirisAPI.dll";

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
		_ClientDomain = mono_domain_create_appdomain("Osiris_Client_Domain", nullptr);

		/* Switch to the client domain */
		mono_domain_set((MonoDomain*)_ClientDomain, true);

		//std::string apiLibraryLocation = NATIVE_API_LIB_LOC "OsirisAPI/OsirisAPI.dll";
		//std::string assemblyPath = inputFile.c_str();

		LoadAssembly(_ClientDomain, &_CoreImage, &_CoreAssembly, "OsirisAPI", coreData);
		LoadAssembly(_ClientDomain, &_ClientImage, &_ClientAssembly, "OsirisGame", clientData);


		/* Search for all valid classes in the DLL and add build the scripted classes */
		const MonoTableInfo* table_info = mono_image_get_table_info((MonoImage*)_CoreImage, MONO_TABLE_TYPEDEF);
		for (int i = 0; i < mono_table_info_get_rows(table_info); i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap((MonoImage*)_CoreImage, cols[MONO_TYPEDEF_NAME]);
			const char* nameSpace = mono_metadata_string_heap((MonoImage*)_CoreImage, cols[MONO_TYPEDEF_NAMESPACE]);

			/** There will mostly be a large number of classes exported due to dependancies,
				However we only care about the OsirisAPI.* namespace
			*/
			if (strcmp(nameSpace, "OsirisAPI") == 0)
			{
				/* Add class to the managed class map */
				_ManagedClasses[name] = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", name);

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
			MonoClass* managedClass = mono_class_from_name((MonoImage*)_ClientImage, "OsirisGame", className.c_str());

			/* Add class to the managed class map */
			_ManagedClasses[className] = mono_class_from_name((MonoImage*)_ClientImage, "OsirisGame", className.c_str());

			std::shared_ptr<ScriptedClass> newScript = std::make_shared<ScriptedClass>(className, &_ManagedClasses[className], _ClientDomain);

			_ScriptedCustomClasses[className] = newScript;
		}

		/* Retrieve fixed objects */
		_InputMousePos = mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Input"]->Properties["MousePos"]->GetGetter(), nullptr, nullptr, nullptr);
		_InputMouseButtonState = mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Input"]->Properties["MouseButtons"]->GetGetter(), nullptr, nullptr, nullptr);
	}


	void Behaviour::BuildManagedGameObjects()
	{
		std::shared_ptr<ScriptedClass> gameObjectClass = _ScriptedClasses["GameObject"];

		for (auto& sl : _CurrentScene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				BuildManagedGameObjects(go.get(), gameObjectClass);
			}
		}
	}

	void Behaviour::BuildManagedGameObjects(GameObject* gameObject, std::shared_ptr<ScriptedClass> gameObjectClass)
	{
		for (auto& go : gameObject->GetGameObjects())
		{
			BuildManagedGameObjects(go.get(), gameObjectClass);
		}

		_ScriptedGameObjects[gameObject->uid] = std::make_shared<ScriptedGameObject>(this, gameObjectClass, gameObject);
	}

	void Behaviour::LinkManagedGameObjects()
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				LinkManagedGameObjects(go.get());
			}
		}
	}

	void Behaviour::LinkManagedGameObjects(GameObject* gameObject)
	{
		for (auto& go : gameObject->GetGameObjects())
		{
			LinkManagedGameObjects(go.get());
		}

		for (auto& component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned class */
				if (scriptComponent->GetClass() != nullptr)
				{
					/* Create the Scripted custom object */
					std::shared_ptr<ScriptedCustomObject> newCustomObject = std::make_shared<ScriptedCustomObject>(_ClientDomain, scriptComponent->GetClass());

					/* Store within the Behaviour Subsystem */
					_ScriptedCustomObjects.emplace(gameObject->uid, newCustomObject);

					/* Set the native game object pointer */
					newCustomObject->SetGameObject(*_ScriptedGameObjects[gameObject->uid]);

					newCustomObject->SetName(scriptComponent->GetClass()->GetName() + "_inst");

					scriptComponent->SetCustomObject(newCustomObject);
				}
			}
		}
	}

	void Behaviour::LinkGameObjectProperties()
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				LinkGameObjectProperties(go.get());
			}
		}
	}

	void Behaviour::LinkGameObjectProperties(GameObject* gameObject)
	{
		for (auto& go : gameObject->GetGameObjects())
		{
			LinkGameObjectProperties(go.get());
		}

		for (auto& component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned class */
				if (scriptComponent->GetClass() != nullptr)
				{
					/* we now need to pass each of the property values to allow UI configured props */
					for (auto& prop : *scriptComponent->Properties)
					{
						prop.second->Set(scriptComponent->GetCustomObject()->Object);
					}
				}
			}
		}
	}

	void Behaviour::StartManagedGameObjects()
	{
		for (auto& sl : _CurrentScene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				StartManagedGameObjects(go.get());
			}
		}
	}

	void Behaviour::StartManagedGameObjects(GameObject* gameObject)
	{
		for (auto& go : gameObject->GetGameObjects())
		{
			StartManagedGameObjects(go.get());
		}

		for (auto& component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned object */
				if (scriptComponent->GetCustomObject() != nullptr)
				{
					MonoObject* exec = nullptr;
					MonoObject* object = (scriptComponent->GetCustomObject())->Object;
					MonoMethod* method = scriptComponent->GetCustomObject()->GetMethod("OnStart");
					mono_runtime_invoke(method, object, nullptr, &exec);

					if (exec != nullptr)
					{
						mono_print_unhandled_exception(exec);
					}
				}
			}
		}
	}

	void Behaviour::UpdateManagedGameObjects(Timestep ts)
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->GetLayers())
		{
			for (auto& go : sl->GetGameObjects())
			{
				UpdateManagedGameObjects(ts, go.get());
			}
		}
	}

	void Behaviour::UpdateManagedGameObjects(Timestep ts, GameObject* gameObject)
	{
		std::vector<void*> updateArgs = std::vector<void*>({ &ts });

		for (auto& go : gameObject->GetGameObjects())
		{
			UpdateManagedGameObjects(ts, go.get());
		}

		/* traverse each of the gameobjects within the scene*/
		for (auto& component : gameObject->components)
		{
			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned object */
				if (scriptComponent->GetCustomObject() != nullptr)
				{
					MonoObject* object = &*scriptComponent->GetCustomObject()->Object;
					MonoMethod* method = &*scriptComponent->GetCustomObject()->GetMethod("OnUpdate");
					mono_runtime_invoke(method, object, &updateArgs[0], nullptr);
				}
			}
		}
	}


	void Behaviour::LoadAssembly(void* domain, void** image, void** assembly, const std::string& ns, std::vector<char>& assemblyData)
	{		
		/* need to add the OsirisAPI dll to the assembly to allow runtime access */
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
			However we only care about the OsirisAPI.* namespace*/
			if (strcmp(nameSpace, ns.c_str()) == 0)
			{
				MonoClass* monoClass;
				monoClass = mono_class_from_name((MonoImage*)(*image), ns.c_str(), name);

				if (monoClass != nullptr)
				{
					//OSR_CORE_TRACE("\tClasses Found: {0}::{1}", ns, name);
				}
			}
		}
	}

	void Behaviour::DebugPrintFunc(const std::string& s)
	{
		OSR_TRACE(s);
	}
}