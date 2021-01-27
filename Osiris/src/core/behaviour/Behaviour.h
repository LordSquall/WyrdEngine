#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Timestep.h"
#include "core/behaviour/CompileResults.h"
#include "core/UID.h"

namespace Osiris
{
	class Scene;
	class Script;
	class Resource;
	class GameObject;
	class ScriptedClass;
	class ScriptedGameObject;
	class ScriptedCustomObject;
	class ScriptedResource;
	class TaskDispatcher;

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
		void SetInputState(std::shared_ptr<GameObject> gameObject, int key, int state);

		void CompileAll(const std::vector<std::string>& files, const std::string& outputDir, const std::string& projectName, CompileResults& results);

		CreateCustomClassResult AddScriptedClassFromFile(const std::string& name, const std::string& filename);

		inline std::shared_ptr<ScriptedClass> GetGameObjectManager() const { return _GameObjectManagerClass; }

		std::shared_ptr<ScriptedClass> GetClass(std::string name);
		std::shared_ptr<ScriptedClass> GetCustomClass(std::string name);
		std::shared_ptr<ScriptedGameObject> GetGameObject(UID uid);
		std::shared_ptr<ScriptedCustomObject> GetCustomObject(UID uid);

		void BroadcastTriggerCall(std::shared_ptr<GameObject> gameObject, std::string& funcName, std::shared_ptr<GameObject> triggerObject, std::vector<void*> args);

		void AddScriptedGameObject(std::shared_ptr<GameObject> gameObject);

		template<class T, typename... Args>
		std::shared_ptr<T> CreateObject(Args&&... x) { return std::make_shared<T>(std::forward<Args>(x)...); }

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

		std::map<UID, std::shared_ptr<ScriptedResource>>	_ScriptedResourceObject;

		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedClasses;
		std::map<std::string, std::shared_ptr<ScriptedClass>>		_ScriptedCustomClasses;

		std::map<UID, std::shared_ptr<ScriptedGameObject>>		_ScriptedGameObjects;
		std::map<UID, std::shared_ptr<ScriptedCustomObject>>	_ScriptedCustomObjects;

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