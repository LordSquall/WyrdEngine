#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/Timestep.h"
#include "core/behaviour/CompileResults.h"
#include "core/UID.h"
#include "core/behaviour/BehaviourInput.h"
#include "core/ecs/ECS.h"
#include "core/interfaces/ISceneManager.h"

#include <mono/metadata/object-forward.h>

namespace Wyrd
{
	/* Forward Declarations */
	class Resources;
	class Scene;
	class Script;
	class Resource;
	class Physics;
	class ScriptedClass;
	class ScriptedCustomObject;
	class ScriptedResource;
	class TaskDispatcher;

	/**
	 * @brief Behaviour class
	 *
	 * This it the primary Behaviour Subsystem class.
	 * Provides management and control over the scripting behaviour engine
	*/
	class WYRD_LIBRARY_API Behaviour
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
		 * @brief Restart the behaviour model of a Scene
		 * @param scene
		*/
		void Restart(std::shared_ptr<Scene> scene);

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
		 * @brief Load a new behavioural model from file locations
		 * @param files
		 * @param inputFile
		 * NOTE: To be moved into external application
		*/
		void LoadBehaviourModel(const std::vector<std::filesystem::path>& files, const std::filesystem::path& inputFile);

		/**
		 * @brief Load a new behavioural model from memory
		 * @param files
		 * @param inputFile
		*/
		void LoadBehaviourModel(const std::vector<std::filesystem::path>& files, std::vector<char>& coreData, std::vector<char>& clientData);

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

		inline void SetSceneManager(ISceneManager* sceneManager) { _SceneManager = sceneManager; }

	private:

		ScriptedCustomObject* GetCustomObject(UID& uid, uint32_t instanceID);

		void UpdateInputState();

		static void DebugPrintFunc(const std::string& s);

		void LoadAssembly(void* domain, void** image, void** assembly, const std::string& ns, std::vector<char>& assemblyData);

	public:
		std::map<Entity, MonoObject*> ScriptedEntities;
		std::map<UID, MonoObject*> ScriptedTextures;


	private:

		/* Managed class map */
		std::map<std::string, MonoClass*>		_ManagedClasses;

		/* Script Classed */
		std::map<std::string, std::shared_ptr<ScriptedClass>>			  _ScriptedClasses;
		std::map<std::string, std::shared_ptr<ScriptedClass>>			  _ScriptedCustomClasses;
		std::map<UID, std::vector<std::shared_ptr<ScriptedCustomObject>>> _ECSScriptedCustomObjects;

		/* Input state and maps*/
		std::map<int, std::string>	_FunctionKeyStateMap;
		void*						_InputMousePos;
		void*						_InputMouseButtonState;
		BehaviourInput				_Input;

		/* State */
		std::shared_ptr<Scene>	_CurrentScene;
		bool					_IsRunning;
		Wyrd::UID				_RunID;
		Scene*					_Scene;

		/* Subsystems */
		Behaviour*		_BehaviourSubsystem;
		Physics*		_PhysicsSubsystem;
		Resources*		_ResourcesSubsystem;

		/* Interfaces */
		ISceneManager*  _SceneManager;

		/* Mono Assembly pointers */
		void*			_RootDomain;
		void*			_ClientDomain;
		void*			_CoreAssembly;
		void*			_ClientAssembly;
		void*			_CoreImage;
		void*			_ClientImage;
	};
}