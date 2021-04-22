#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Timestep.h"
#include "core/behaviour/CompileResults.h"
#include "core/UID.h"
#include "core/behaviour/BehaviourInput.h"

#include <mono/metadata/object-forward.h>

#ifndef NATIVE_API_LIB_LOC
#define NATIVE_API_LIB_LOC "C:/Projects/Osiris/OsirisEngine/lib/Debug/"
#endif

namespace Osiris
{
	/* Forward Declarations */
	class Resources;
	class Scene;
	class Script;
	class Resource;
	class Physics;
	class GameObject;
	class ScriptedClass;
	class ScriptedGameObject;
	class ScriptedCustomObject;
	class ScriptedResource;
	class TaskDispatcher;

	/**
	 * @brief Behaviour class
	 *
	 * This it the primary Behaviour Subsystem class.
	 * Provides management and control over the scripting behaviour engine
	*/
	class OSR_LIBRARY_API Behaviour
	{
	public:
		/**
		 * @brief Constructor
		*/
		Behaviour();

		/**
		 * @brief Destructor
		*/
		virtual ~Behaviour();

	public:
		/**
		 * @brief Start the behaviour model of a Scene
		 * @param scene 
		*/
		void Start(std::shared_ptr<Scene> scene);

		/**
		 * @brief Stops the current behaviour model
		*/
		void Stop();

		/**
		 * @brief Update the current behaviour model
		 * @param ts 
		*/
		void Update(Timestep ts);

		/**
		 * @brief Set the current input key state of the simluation
		 * @param key 
		 * @param state 
		*/
		void SetInputState(int key, int state);

		/**
		 * @brief Set the current input mouse state of the simluation
		 * @param xPos
		 * @param yPos
		*/
		void SetMouseState(float xPos, float yPos);

		/**
		 * @brief Set the current input mouse button state of the simluation
		 * @param i
		 * @param state
		*/
		void SetMouseButtonState(int i, bool state);

		/**
		 * @brief Compiles all the supplied script files into a runnable behavioural model
		 * @param files 
		 * @param outputFile
		 * @param results 
		*/
		void CompileAll(const std::vector<std::string>& files, const std::string& outputFile, CompileResults& results);

		/**
		 * @brief Load a new behavioural model from file locations
		 * @param files
		 * @param inputFile
		 * NOTE: To be moved into external application
		*/
		void LoadBehaviourModel(const std::vector<std::string>& files, const std::string& inputFile);

		/**
		 * @brief Load a new behavioural model from memory
		 * @param files
		 * @param inputFile
		*/
		void LoadBehaviourModel(const std::vector<std::string>& files, std::vector<char>& coreData, std::vector<char>& clientData);

		/**
		 * @brief Get the domain context pointer
		 * @return 
		*/
		void* GetDomain();

		/**
		 * @brief Retreive a Core behaviour class model by name.
		 * @param name 
		 * @return 
		*/
		std::shared_ptr<ScriptedClass> GetClass(std::string name);

		/**
		 * @brief Retrieve a User behaviour class model by name.
		 * @param name 
		 * @return 
		*/
		std::shared_ptr<ScriptedClass> GetCustomClass(std::string name);

		/**
		 * @brief Retrieve a User behaviour class model by uid.
		 * @param uid 
		 * @return 
		*/
		std::shared_ptr<ScriptedClass> GetCustomClassByUID(UID& uid);

		/**
		 * @brief Retrieve a GameObject reference from the behaviour model by UID
		 * @param uid 
		 * @return 
		*/
		std::shared_ptr<ScriptedGameObject> GetGameObject(UID uid);

		/**
		 * @brief Retreive an custom object instance from the behaviour model by UID
		 * @param uid 
		 * @return 
		*/
		std::shared_ptr<ScriptedCustomObject> GetCustomObject(UID uid);

		/**
		 * @brief Add a Scripted Game Object to the bahaviour subsystem
		 * @param uid
		 * @param gameObject
		 * @param managedObject
		*/
		void AddScriptedGameObject(UID uid, GameObject* gameObject, void* managedObject = nullptr);

		/**
		 * @brief Add a Scripted Custom Object to the bahaviour subsystem
		 * @param uid 
		 * @param customObject 
		*/
		void AddScriptedCustomObject(UID uid, std::shared_ptr<ScriptedCustomObject> customObject);

		/**
		 * @brief Boardcast a function call to all scripts on a GameObject 
		 * @param gameObject 
		 * @param funcName 
		 * @param triggerObject 
		 * @param args 
		*/
		//TODO: Rename to generic function name
		void BroadcastTriggerCall(std::shared_ptr<GameObject> gameObject, std::string& funcName, std::shared_ptr<GameObject> triggerObject, std::vector<void*> args);

	private:
		void SetInputState(GameObject* gameObject, int key, int state);

		void BuildManagedGameObjects();
		void BuildManagedGameObjects(GameObject* gameObject, std::shared_ptr<ScriptedClass> gameObjectClass);

		void LinkManagedGameObjects();
		void LinkManagedGameObjects(GameObject* gameObject);

		void LinkGameObjectProperties();
		void LinkGameObjectProperties(GameObject* gameObject);

		void StartManagedGameObjects();
		void StartManagedGameObjects(GameObject* gameObject);

		void UpdateManagedGameObjects(Timestep ts);
		void UpdateManagedGameObjects(Timestep ts, GameObject* gameObject);

		void UpdateInputState();

		static void DebugPrintFunc(const std::string& s);

		void LoadAssembly(void* domain, void** image, void** assembly, const std::string& ns, std::vector<char>& assemblyData);

	private:
		std::map<UID, std::shared_ptr<ScriptedResource>>	_ScriptedResourceObject;

		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedClasses;
		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedCustomClasses;

		std::map<UID, std::shared_ptr<ScriptedGameObject>>		_ScriptedGameObjects;
		std::map<UID, std::shared_ptr<ScriptedCustomObject>>	_ScriptedCustomObjects;

		std::map<int, std::string> _FunctionKeyStateMap;

		void* _InputMousePos;
		void* _InputMouseButtonState;

		std::map<std::string, MonoClass*>		_ManagedClasses;

		std::shared_ptr<Scene>	_CurrentScene;
		bool					_IsRunning;

		Behaviour*		_BehaviourSubsystem;
		Physics*		_PhysicsSubsystem;
		Resources*		_ResourcesSubsystem;
		
		std::vector<void*> _NativePtrMap;

		BehaviourInput	_Input;

		void*			_RootDomain;
		void*			_ClientDomain;

		void*			_CoreAssembly;
		void*			_ClientAssembly;
		void*			_CoreImage;
		void*			_ClientImage;
	};
}