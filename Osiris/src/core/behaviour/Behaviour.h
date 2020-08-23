#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/Timestep.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/ScriptedCustomObject.h"

namespace Osiris
{
	class Scene;
	class Script;
	class GameObject;

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

		CreateCustomClassResult AddScriptedClassFromFile(const std::string& name, const std::string& filename);

		inline std::shared_ptr<ScriptedClass> GetGameObjectManager() const { return _GameObjectManagerClass; }

		std::shared_ptr<ScriptedClass> GetClass(std::string name);
		std::shared_ptr<ScriptedClass> GetCustomClass(std::string name);
		std::shared_ptr<ScriptedGameObject> GetGameObject(uint32_t uid);
		std::shared_ptr<ScriptedCustomObject> GetCustomObject(uint32_t uid);
	private:
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
		void*					_Domain;
	};
}