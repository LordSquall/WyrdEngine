#pragma once

#include "osrpch.h"
#include "SimulationService.h"

#include "services/ServiceManager.h"

#include <glm/glm.hpp>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>
#include <iostream>

namespace Osiris::Editor
{
	lua_State* _LuaState;
	GameObject* _CurrentGO;
	std::shared_ptr<luabridge::LuaRef> _CurrentGORef;
	Timestep _Timestep;

	void SimulationService::DebugPrintFunc(const std::string& s)
	{
		OSR_TRACE(s);
	}

	void SimulationService::OnCreate()
	{
		/* cache services */
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		
		/* setup the global state for lua  */
		_LuaState = luaL_newstate();
		luaL_openlibs(_LuaState);

		/* build the debug namespace */
		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Console")
			.addFunction("print", SimulationService::DebugPrintFunc)
			.endNamespace();

		/* build the color class*/
		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Engine")
			.beginClass<glm::vec3>("Color")
			.addProperty<float>("r", &glm::vec3::r)
			.addProperty<float>("g", &glm::vec3::g)
			.addProperty<float>("b", &glm::vec3::b)
			.endClass()
			.endNamespace();

		/* build the sprint component class*/
		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Engine")
			.beginClass<Timestep>("Timestep")
			.addProperty<float>("delta", &Timestep::GetMilliseconds)
			.endClass()
			.endNamespace();

		/* build the sprint component class*/
		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Engine")
			.beginClass<SpriteComponent>("Sprite")
			.addProperty<glm::vec3>("color", &SpriteComponent::Color)
			.addFunction("__tostring", &SpriteComponent::toString)
			.endClass()
			.endNamespace();

		/* build the gameobject class*/
		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Engine")
			.beginClass<GameObject>("GameObject")
			.addConstructor<void (*)(void)>()
			.addProperty<std::string>("name", &GameObject::name)
			.addProperty<SpriteComponent>("sprite", &GameObject::spriteRender)
			.endClass()
			.endNamespace();

		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("_go")
			.addVariable<GameObject*>("current", &_CurrentGO, false)
			.endNamespace();

		luabridge::getGlobalNamespace(_LuaState)
			.beginNamespace("Time")
			.addVariable<Timestep>("Current", &_Timestep, false)
			.endNamespace();

		_CurrentGORef = std::make_shared<luabridge::LuaRef>(luabridge::getGlobal(_LuaState, "_go"));

		_IsRunning = false;
	}

	void SimulationService::OnDestroy()
	{
	}

	void SimulationService::Start()
	{
		OSR_TRACE("Starting Simulation Service");

		_IsRunning = true;

		_CurrentScene = _WorkspaceService->GetLoadedScene();

		if (_CurrentScene != nullptr)
		{
			/* Query each of the layer scene objects */
			for (auto& sl : _CurrentScene->layers2D)
			{
				for (auto& go : sl->gameobjects)
				{
					/* Set the current gameobject in the lua global table */
					_CurrentGO = &*go;

					if (go->script.GetScriptResource() != nullptr)
					{
						(*go->script.GetScriptResource()->ScriptedObjectTemplate->OnStartFunc)();
					}
				}
			}
		}
	}

	void SimulationService::Stop()
	{
		OSR_TRACE("Stopping Simulation Service");

		_IsRunning = false;
	}

	void SimulationService::Update(Timestep ts)
	{
		_Timestep = ts;
		if (_IsRunning == true)
		{
			if (_CurrentScene != nullptr)
			{
				/* Query each of the layer scene objects */
				for (auto& sl : _CurrentScene->layers2D)
				{
					for (auto& go : sl->gameobjects)
					{
						/* Set the current gameobject in the lua global table */
						_CurrentGO = &*go;

						if (go->script.GetScriptResource() != nullptr)
						{
							(*go->script.GetScriptResource()->ScriptedObjectTemplate->OnUpdateFunc)();
						}
					}
				}
			}
		}
	}

	SimulationService::RunReturnVal SimulationService::RunScript(const std::string& src)
	{
		int res = luaL_dostring(_LuaState, src.c_str());

		if (res != LUA_OK)
		{
			return RunReturnVal{ false, lua_tostring(_LuaState, -1) };
		}

		return RunReturnVal{ true, "" };
	}

	SimulationService::RunReturnVal SimulationService::RunScriptFile(const std::string& path)
	{
		int res = luaL_dofile(_LuaState, path.c_str());

		if (res != LUA_OK)
		{
			return RunReturnVal{ false, lua_tostring(_LuaState, -1) };
		}

		return RunReturnVal{ true, "" };
	}


	SimulationService::CreateReturnVal SimulationService::CreateScriptableObjectTemplate(const std::string& source)
	{
		std::shared_ptr<ScriptedObjectTemplate> newTemplate = std::make_shared<ScriptedObjectTemplate>();

		/* run the script in the lua context */
		RunReturnVal runResult = RunScript(source);

		if (runResult.result == true)
		{
			newTemplate->OnStartFunc = std::make_shared<luabridge::LuaRef>(luabridge::getGlobal(_LuaState, "onStart"));
			newTemplate->OnUpdateFunc = std::make_shared<luabridge::LuaRef>(luabridge::getGlobal(_LuaState, "onUpdate"));

			return CreateReturnVal{ true, "", newTemplate };
		}
		else
		{
			return CreateReturnVal{ false, runResult.error, newTemplate };
		}
	}
}