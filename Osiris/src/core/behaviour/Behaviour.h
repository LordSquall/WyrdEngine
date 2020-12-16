#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Timestep.h"

namespace Osiris
{
	class Scene;
	class Script;
	class GameObject;
	class ScriptedClass;
	class ScriptedGameObject;
	class ScriptedCustomObject;

	class OSR_LIBRARY_API Behaviour
	{
	public:
		struct CreateCustomClassResult
		{
			bool success;
			std::string message;
			std::shared_ptr<ScriptedClass> customClass;
		};

		struct CreateCustomObjectResult
		{
			bool success;
			std::string message;
			std::shared_ptr<ScriptedCustomObject> customObject;
		};

	public:
		Behaviour();
		virtual ~Behaviour();

	public:
		void Start(std::shared_ptr<Scene> scene);
		void Stop();
		void Update(Timestep ts);

		void SetInputState(int key, int state);

		void CompileAll(const std::vector<std::string>& files, const std::string& outputDir, const std::string& projectName);

		CreateCustomClassResult AddScriptedClassFromFile(const std::string& name, const std::string& filename);

		inline std::shared_ptr<ScriptedClass> GetGameObjectManager() const { return _GameObjectManagerClass; }

		std::shared_ptr<ScriptedClass> GetClass(std::string name);
		std::shared_ptr<ScriptedClass> GetCustomClass(std::string name);
		std::shared_ptr<ScriptedGameObject> GetGameObject(uint32_t uid);
		std::shared_ptr<ScriptedCustomObject> GetCustomObject(uint32_t uid);

		void BroadcastTriggerCall(std::shared_ptr<GameObject> gameObject, std::string& funcName, std::shared_ptr<GameObject> triggerObject, std::vector<void*> args);

	private:
		void BuildManagedGameObjects();
		void BuildManagedGameObjects(std::shared_ptr<GameObject> gameObject, std::shared_ptr<ScriptedClass> gameObjectClass);

		void LinkManagedGameObjects();
		void LinkManagedGameObjects(std::shared_ptr<GameObject> gameObject);

		void LinkGameObjectProperties();
		void LinkGameObjectProperties(std::shared_ptr<GameObject> gameObject);

		void UpdateManagedGameObjects(Timestep ts);
		void UpdateManagedGameObjects(Timestep ts, std::shared_ptr<GameObject> gameObject);

		static void DebugPrintFunc(const std::string& s);

	private:
		std::shared_ptr<ScriptedClass>		_GameObjectManagerClass;

		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedClasses;
		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedCustomClasses;

		std::map<uint32_t, std::shared_ptr<ScriptedGameObject>>		_ScriptedGameObjects;
		std::map<uint32_t, std::shared_ptr<ScriptedCustomObject>>	_ScriptedCustomObjects;

		std::map<int, std::string> _FunctionKeyStateMap;

		std::shared_ptr<Scene>	_CurrentScene;
		bool					_IsRunning;

		void*			_Domain;
		void*			_CoreAssembly;
		void*			_ClientAssembly;
		void*			_CoreImage;
		void*			_ClientImage;
	};
}