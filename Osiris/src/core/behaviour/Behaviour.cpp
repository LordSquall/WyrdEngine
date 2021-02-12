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

#ifndef NATIVE_API_LIB_LOC
#define NATIVE_API_LIB_LOC "C:/Projects/Osiris/OsirisEngine/lib/Debug/"
#endif

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
		_ResourcesSubsystem = Application::Get().GetResourcesPtr();

		/* pass in the subsystem pointers to the managed domain */
		MonoClass* monoClass;
		monoClass = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", "SubsystemManager");

		MonoProperty* behaviourProp = mono_class_get_property_from_name(monoClass, "Behaviour");
		MonoProperty* resourcesProp = mono_class_get_property_from_name(monoClass, "Resources");

		std::vector<void*> behaviourArgs = std::vector<void*>({ &_BehaviourSubsystem });
		mono_property_set_value(behaviourProp, nullptr, &behaviourArgs[0], nullptr);


		std::vector<void*> resourceArgs = std::vector<void*>({ &_ResourcesSubsystem });
		mono_property_set_value(resourcesProp, nullptr, &resourceArgs[0], nullptr);

		/* first stage is to create a instance of each gameobject within mono */
		if (_CurrentScene != nullptr)
		{
			BuildManagedGameObjects();

			/* next stage to build hierarchy in between GameObjects */
			BuildManagedGameObjectHierarchy();

			/* next stage is to pass over any script components and create custom objects for each one */
			LinkManagedGameObjects();

			/* next stage is to pass over any script components and link properties */
			LinkGameObjectProperties();

			/* Query each of the layer scene objects */
			for (auto& sl : _CurrentScene->layers2D)
			{
				for (auto& go : sl->children)
				{					
					for (auto component : go->components)
					{
						if (component->GetType() == SceneComponentType::ScriptComponent)
						{
							ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

							/* Check if we have an assigned object */
							if (scriptComponent->GetCustomObject() != nullptr)
							{
								MonoObject* object = (scriptComponent->GetCustomObject())->Object;
								MonoMethod* method = scriptComponent->GetCustomObject()->GetMethod("OnStart");
								mono_runtime_invoke(method, object, nullptr, nullptr);
							}
						}
					}
				}
			}
		}
	}

	void Behaviour::Stop()
	{
		/* clear managed objects */
		_ScriptedGameObjects.clear();
		_ScriptedCustomObjects.clear();

		mono_runtime_invoke(MonoUtils::FindMethodInClass(GetClass("GameObjectManager"), "Reset", 0, true), NULL, NULL, NULL);

		_IsRunning = false;
	}

	void Behaviour::Update(Timestep ts)
	{
		/* we only action anything in the function if we are in running mode */
		if (_IsRunning == true)
		{
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
				for (auto& sl : _CurrentScene->layers2D)
				{
					for (auto& go : sl->children)
					{
						SetInputState(go, key, state);
					}
				}
			}
		}
	}

	void Behaviour::SetInputState(std::shared_ptr<GameObject> object, int key, int state)
	{
		for (auto& component : object->components)
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

		for (auto& go : object->children)
		{
			SetInputState(go, key, state);
		}
	}

	void Behaviour::SetMouseState(float xPos, float yPos)
	{
		std::vector<void*> args;
		args.push_back(&xPos);
		args.push_back(&yPos);

		mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Vector2"]->Properties["X"]->GetSetter(), (MonoObject*)_InputMousePos, &args[0], nullptr);
		mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Vector2"]->Properties["Y"]->GetSetter(), (MonoObject*)_InputMousePos, &args[1], nullptr);
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

	std::shared_ptr<ScriptedGameObject> Behaviour::GetGameObject(UID uid)
	{ 
		return _ScriptedGameObjects[uid]; 
	}

	std::shared_ptr<ScriptedCustomObject> Behaviour::GetCustomObject(UID uid)
	{ 
		return _ScriptedCustomObjects[uid]; 
	}


	void Behaviour::AddScriptedGameObject(UID uid, std::shared_ptr<GameObject> gameObject)
	{
		_ScriptedGameObjects[uid] = std::make_shared<ScriptedGameObject>(this, _ScriptedClasses["GameObject"], gameObject);
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
		OSR_TRACE("COMPILING BEHAVIOUR MODEL!!!");

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

		std::string apiLibraryLocation = NATIVE_API_LIB_LOC "OsirisAPI/OsirisAPI.dll";
		std::string assemblyPath = inputFile.c_str();

		LoadAssembly(_ClientDomain, &_CoreImage, &_CoreAssembly, "OsirisAPI", apiLibraryLocation);
		LoadAssembly(_ClientDomain, &_ClientImage, &_ClientAssembly, "OsirisGame", assemblyPath);


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

			/* Add class to the managed class map */
			_ManagedClasses[className] = mono_class_from_name((MonoImage*)_ClientImage, "OsirisGame", className.c_str());

			std::shared_ptr<ScriptedClass> newScript = std::make_shared<ScriptedClass>(className, &_ManagedClasses[className], _ClientDomain);

			_ScriptedCustomClasses[className] = newScript;
		}

		/* Retrieve fixed objects */
		_InputMousePos = mono_runtime_invoke((MonoMethod*)_ScriptedClasses["Input"]->Properties["MousePos"]->GetGetter(), nullptr, nullptr, nullptr);
	}

	void Behaviour::BuildManagedGameObjects()
	{
		std::shared_ptr<ScriptedClass> gameObjectClass = _ScriptedClasses["GameObject"];

		for (auto& sl : _CurrentScene->layers2D)
		{
			for (auto& go : sl->children)
			{
				BuildManagedGameObjects(go, gameObjectClass);
			}
		}
	}

	void Behaviour::BuildManagedGameObjects(std::shared_ptr<GameObject> gameObject, std::shared_ptr<ScriptedClass> gameObjectClass)
	{
		for (auto& go : gameObject->children)
		{
			BuildManagedGameObjects(go, gameObjectClass);
		}

		_ScriptedGameObjects[gameObject->uid] = std::make_shared<ScriptedGameObject>(this, gameObjectClass, gameObject);
	}

	void Behaviour::BuildManagedGameObjectHierarchy()
	{
		std::shared_ptr<ScriptedClass> gameObjectClass = _ScriptedClasses["GameObject"];

		for (auto& sgo : _ScriptedGameObjects)
		{
			auto parentGameObject = sgo.second->GetGameObject()->parent;

			auto scriptParentGameObject = _ScriptedGameObjects[parentGameObject->uid];

			if (scriptParentGameObject != nullptr)
			{
				std::vector<void*> args;
				args.push_back(sgo.second->Object);

				mono_runtime_invoke(gameObjectClass->Methods["_AddGameObject"]->GetManagedMethod(), (MonoObject*)scriptParentGameObject->Object, &args[0], nullptr);
			}

		}
	}

	void Behaviour::LinkManagedGameObjects()
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->layers2D)
		{
			for (auto& go : sl->children)
			{
				LinkManagedGameObjects(go);
			}
		}
	}

	void Behaviour::LinkManagedGameObjects(std::shared_ptr<GameObject> gameObject)
	{
		for (auto& go : gameObject->children)
		{
			LinkManagedGameObjects(go);
		}

		for (auto component : gameObject->components)
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
					newCustomObject->SetGameObject(_ScriptedGameObjects[gameObject->uid]);

					newCustomObject->SetName(scriptComponent->GetClass()->GetName() + "_inst");

					scriptComponent->SetCustomObject(newCustomObject);
				}
			}
		}
	}

	void Behaviour::LinkGameObjectProperties()
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->layers2D)
		{
			for (auto& go : sl->children)
			{
				LinkGameObjectProperties(go);
			}
		}
	}

	void Behaviour::LinkGameObjectProperties(std::shared_ptr<GameObject> gameObject)
	{
		for (auto& go : gameObject->children)
		{
			LinkGameObjectProperties(go);
		}

		for (auto component : gameObject->components)
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

	void Behaviour::UpdateManagedGameObjects(Timestep ts)
	{
		/* Query each of the layer scene objects */
		for (auto& sl : _CurrentScene->layers2D)
		{
			for (auto& go : sl->children)
			{
				UpdateManagedGameObjects(ts, go);
			}
		}
	}

	void Behaviour::UpdateManagedGameObjects(Timestep ts, std::shared_ptr<GameObject> gameObject)
	{
		for (auto& go : gameObject->children)
		{
			UpdateManagedGameObjects(ts, go);
		}

		/* traverse each of the gameobjects within the scene*/
		for (auto& component : gameObject->components)
		{
			if (component->Initialised == false)
			{
				component->Initialise();
			}

			if (component->GetType() == SceneComponentType::ScriptComponent)
			{
				ScriptComponent* scriptComponent = (ScriptComponent*)&*component;

				/* Check if we have an assigned object */
				if (scriptComponent->GetCustomObject() != nullptr)
				{
					MonoObject* object = &*scriptComponent->GetCustomObject()->Object;
					MonoMethod* method = &*scriptComponent->GetCustomObject()->GetMethod("OnUpdate");
					mono_runtime_invoke(method, object, nullptr, nullptr);
				}
			}
		}
	}


	void Behaviour::LoadAssembly(void* domain, void** image, void** assembly, const std::string& ns, const std::string& filepath)
	{
		OSR_CORE_TRACE("Loading assembly {0} into {1}", filepath, mono_domain_get_friendly_name((MonoDomain*)domain));

		//open file
		std::ifstream input(filepath.c_str(), std::ios::binary);
		std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
		input.close(); 
		
		/* need to add the OsirisAPI dll to the assembly to allow runtime access */
		MonoImageOpenStatus status = MONO_IMAGE_ERROR_ERRNO;
		
		
		(*image) = mono_image_open_from_data(&buffer[0], buffer.size(), 1, &status); 
		if (status != MONO_IMAGE_OK || (*image) == NULL)
		{
			return;
		}
		
		(*assembly) = mono_assembly_load_from((MonoImage*)(*image), filepath.c_str(), &status);
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
					OSR_CORE_TRACE("\tClasses Found: {0}::{1}", ns, name);
				}
			}
		}
	}

	void Behaviour::DebugPrintFunc(const std::string& s)
	{
		OSR_TRACE(s);
	}
}