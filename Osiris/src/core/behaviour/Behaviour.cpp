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

	Behaviour::Behaviour() : _IsRunning(false)
	{
		std::string apiLibraryLocation = NATIVE_API_LIB_LOC "OsirisAPI/OsirisAPI.dll";
		std::string monoLibraryDirectory = MONO_INSTALL_LOC "lib";
		std::string monoExtensionDirectory = MONO_INSTALL_LOC "etc";

		mono_set_dirs(monoLibraryDirectory.c_str(),
			monoExtensionDirectory.c_str());

		/* intiailise the domain */
		_Domain = mono_jit_init("MonoScriptTry");
		if (!_Domain)
		{
			std::cout << "mono_jit_init failed" << std::endl;
			system("pause");
		}
		
		/* need to add the OsirisAPI dll to the assembly to allow runtime access */
		std::string assemblyPath = (apiLibraryLocation);
		_CoreAssembly = mono_domain_assembly_open((MonoDomain*)_Domain, assemblyPath.c_str());
		if (!_CoreAssembly)
		{
			OSR_CORE_ERROR("mono_domain_assembly_open failed for {0}", assemblyPath);
			system("pause");
		}

		/* Create image */
		_CoreImage = mono_assembly_get_image((MonoAssembly*)_CoreAssembly);
		if (!_CoreImage)
		{
			OSR_CORE_ERROR("mono_assembly_get_image failed for {0}", assemblyPath);
			system("pause");
		}


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
				MonoClass* monoClass;
				monoClass = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", name);

				/* Create a new Scripted Class */
				std::shared_ptr<ScriptedClass> newScriptedClass = std::make_shared<ScriptedClass>(name, monoClass, _Domain);

				/* Store within the Behaviour Subsystem */
				_ScriptedClasses.emplace(name, newScriptedClass);
			}
		}

		/* Output all the classes found in the OsirisAPI dll */
		OSR_CORE_TRACE("Classes found in OsirisAPI.dll:");
		for(auto scriptedClass : _ScriptedClasses)
		{
			OSR_CORE_TRACE("\t{0}", scriptedClass.second->GetName());
		}

		/* Retrieve the Script side management classes */
		MonoClass* monoClass;
		monoClass = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", "GameObjectManager");
		_GameObjectManagerClass = std::make_shared<ScriptedClass>("GameObjectManager", monoClass, _Domain);

		/* Build up mapping between Osiris Key States and managed key state functions */
		_FunctionKeyStateMap[0] = "OnKeyPressed";
		_FunctionKeyStateMap[1] = "OnKeyDown";
		_FunctionKeyStateMap[2] = "OnKeyUp";
	}

	Behaviour::~Behaviour()
	{
		mono_jit_cleanup((MonoDomain*)_Domain);
	}

	void Behaviour::Start(std::shared_ptr<Scene> scene)
	{
		_CurrentScene = scene;
		_IsRunning = true;

		/* pass in the subsystem pointers to the managed domain */
		MonoClass* monoClass;
		monoClass = mono_class_from_name((MonoImage*)_CoreImage, "OsirisAPI", "SubsystemManager");

		MonoProperty* prop = mono_class_get_property_from_name(monoClass, "Behaviour");

		/* build arg list for the key event functions */
		std::vector<void*> args = std::vector<void*>({ &Application::Get().GetBehaviour() });
		mono_property_set_value(prop, nullptr, &args[0], nullptr);

		/* first stage is to create a instance of each gameobject within mono */
		if (_CurrentScene != nullptr)
		{
			BuildManagedGameObjects();
		}

		/* next stage is to pass over any script components and create custom objects for each one */
		if (_CurrentScene != nullptr)
		{
			LinkManagedGameObjects();
		}

		/* next stage is to pass over any script components and link properties */
		if (_CurrentScene != nullptr)
		{
			LinkGameObjectProperties();
		}

		if (_CurrentScene != nullptr)
		{
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

	Behaviour::CreateCustomClassResult Behaviour::AddScriptedClassFromFile(const std::string& name, const std::string& filename)
	{
		return Behaviour::CreateCustomClassResult{ true, "", nullptr };
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

	void Behaviour::CompileAll(const std::vector<std::string>& files, const std::string& outputDir, const std::string& projectName, CompileResults& results)
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
		command += "-out:" + outputDir + "\\" + projectName;

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

		/* Create Assembly */
		MonoAssembly* monoAssembly;
		std::string assemblyPath = (outputDir + "\\" + projectName).c_str();
		monoAssembly = mono_domain_assembly_open((MonoDomain*)_Domain, assemblyPath.c_str());
		if (!monoAssembly)
		{
			OSR_CORE_ERROR("mono_domain_assembly_open failed for {0}", assemblyPath);
			system("pause");
		}

		/* Create image */
		MonoImage* monoImage = mono_assembly_get_image(monoAssembly);
		if (!monoImage)
		{
			OSR_CORE_ERROR("mono_assembly_get_image failed for {0}", assemblyPath);
			system("pause");
		}

		/* Create each of the new scripted classes */
		for (auto& file : files)
		{
			// Get the file name
			std::filesystem::path filename = file;
			std::string className = filename.stem().string();
			MonoClass* monoClass;
			monoClass = mono_class_from_name(monoImage, "", className.c_str());

			if (!monoClass)
			{
				OSR_CORE_ERROR("mono_class_from_name failed for {0}", className);
				system("pause");
			}

			std::shared_ptr<ScriptedClass> newScript = std::make_shared<ScriptedClass>(className, monoClass, _Domain);

			_ScriptedCustomClasses.emplace(className, newScript);
		}
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

		_ScriptedGameObjects[gameObject->uid] = std::make_shared<ScriptedGameObject>(_Domain, gameObjectClass, gameObject);
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
					std::shared_ptr<ScriptedCustomObject> newCustomObject = std::make_shared<ScriptedCustomObject>(_Domain, scriptComponent->GetClass());

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
					for (auto prop : scriptComponent->Properties)
					{
						std::shared_ptr<GameObject> go;
						std::shared_ptr<ScriptComponent> sc;
						std::vector<void*> args;

						switch (prop.type)
						{
						case PropType::INT:
							args.push_back(&prop.intVal);

							mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
							break;
						case PropType::FLOAT:
							args.push_back(&prop.floatVal);

							mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
							break;
						case PropType::STRING:
							args.push_back(mono_string_new((MonoDomain*)_Domain, prop.stringVal.c_str()));

							mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
							break;
						case PropType::GAMEOBJECT:
							// find matching script component class type
							if (prop.objectClassNameVal == "OsirisAPI.GameObject")
							{
								go = _CurrentScene->FindGameObject(prop.objectUID);
								if (go != nullptr)
								{
									if (_ScriptedGameObjects.find(go->uid) != _ScriptedGameObjects.end())
									{
										args.push_back(_ScriptedGameObjects[go->uid]->Object);
									}
									else
									{
										args.push_back(nullptr);
									}

									mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
								}
							}
							else
							{
								go = _CurrentScene->FindGameObject(prop.objectUID);
								sc = go->FindScriptComponent(prop.objectClassNameVal);
								args.push_back(sc->GetCustomObject()->Object);

								mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
							}
							break;
						case PropType::COLOR:
						{
							MonoClass* colorClass = (MonoClass*)_ScriptedClasses["Color"]->ManagedClass;
							MonoObject* colorObject = mono_object_new((MonoDomain*)_Domain, colorClass);

							MonoProperty* rProperty = mono_class_get_property_from_name(colorClass, "R");
							MonoProperty* gProperty = mono_class_get_property_from_name(colorClass, "G");
							MonoProperty* bProperty = mono_class_get_property_from_name(colorClass, "B");
							MonoProperty* aProperty = mono_class_get_property_from_name(colorClass, "A");

							MonoMethod* rPropSetter = mono_property_get_set_method(rProperty);
							MonoMethod* gPropSetter = mono_property_get_set_method(gProperty);
							MonoMethod* bPropSetter = mono_property_get_set_method(bProperty);
							MonoMethod* aPropSetter = mono_property_get_set_method(aProperty);

							args.push_back(&prop.colorVal.r);
							args.push_back(&prop.colorVal.g);
							args.push_back(&prop.colorVal.b);
							args.push_back(&prop.colorVal.a);

							mono_runtime_invoke(rPropSetter, colorObject, &args[0], nullptr);
							mono_runtime_invoke(gPropSetter, colorObject, &args[1], nullptr);
							mono_runtime_invoke(bPropSetter, colorObject, &args[2], nullptr);
							mono_runtime_invoke(aPropSetter, colorObject, &args[3], nullptr);

							args.clear();

							args.push_back(colorObject);

							mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
						}
						break;
						case PropType::TEXTURE:
						{
							MonoClass* textureClass = (MonoClass*)_ScriptedClasses["Texture"]->ManagedClass;
							MonoObject* textureObject = mono_object_new((MonoDomain*)_Domain, textureClass);
							MonoProperty* property = mono_class_get_property_from_name(textureClass, "NativePtr");
							MonoMethod* propSetter = mono_property_get_set_method(property);

							std::shared_ptr<Texture> texture = Application::Get().GetResources().Textures[prop.objectUID];

							prop.objectVal = &*texture;

							args.push_back(&prop.objectVal);

							mono_runtime_invoke(propSetter, textureObject, &args[0], nullptr);

							args.clear();

							args.push_back(textureObject);

							mono_runtime_invoke((MonoMethod*)prop.setter, scriptComponent->GetCustomObject()->Object, &args[0], nullptr);
						}
						break;
						default:
							OSR_CORE_TRACE("Unknown Type. Skipping Property serialisation!");
							break;
						}
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

	void Behaviour::AddScriptedGameObject(std::shared_ptr<GameObject> gameObject)
	{
		_ScriptedGameObjects[gameObject->uid] = std::make_shared<ScriptedGameObject>(_Domain, _ScriptedClasses["GameObject"], gameObject);
	}

	void Behaviour::DebugPrintFunc(const std::string& s)
	{
		OSR_TRACE(s);
	}
}