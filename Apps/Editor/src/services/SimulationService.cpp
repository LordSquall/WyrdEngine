#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/SpriteComponent.h>

/* local includes */
#include "SimulationService.h"
#include "services/ServiceManager.h"
#include "views/SceneViewer/SceneViewer.h"

/* external includes */
#include <glm/glm.hpp>
#include <iostream>

namespace Osiris::Editor
{
	void SimulationService::OnCreate()
	{
		/* cache services */
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		/* Subscribe to project events */
		_EventService->Subscribe(Events::EventType::BuildBehaviourModel, EVENT_FUNC(SimulationService::OnBuildBehaviourModelEvent));

		_IsRunning = false;
	}

	void SimulationService::OnDestroy()
	{
	}

	void SimulationService::OnUpdate()
	{
		if (_pendingRebuild)
		{
			_ResourceService->BuildScripts();
			_pendingRebuild = false;
		}
	}

	void SimulationService::Start()
	{
		OSR_TRACE("Starting Simulation Service");

		/* ensure the scene is saved before starting the simulation */
		_WorkspaceService->SaveScene();

		_IsRunning = true;

		Application::Get().GetBehaviour().Start(_WorkspaceService->GetLoadedScene());
		Application::Get().GetPhysics().Start(_WorkspaceService->GetLoadedScene());

	}

	void SimulationService::Stop()
	{
		Application::Get().GetPhysics().Stop();
		Application::Get().GetBehaviour().Stop();

		_WorkspaceService->ReloadScene();

		OSR_TRACE("Stopping Simulation Service");

		_IsRunning = false;
	}

	void SimulationService::Update(Timestep ts)
	{
		/* we only action anything in the function if we are in running mode */
		if (_IsRunning == true)
		{
			auto sceneViewer = std::dynamic_pointer_cast<SceneViewer>(_SceneViewer);

			glm::vec2 worldSpaceCoords = sceneViewer->GetWorldSpaceFromPoint(glm::vec2(_MousePos[0], _MousePos[1]));
			Application::Get().GetBehaviour().SetMouseState(worldSpaceCoords.x, worldSpaceCoords.y);

			Application::Get().GetBehaviour().Update(ts);
			Application::Get().GetPhysics().Update(ts);

			_WorkspaceService->GetLoadedScene()->Update();
		}
	}


	void SimulationService::OnBuildBehaviourModelEvent(Events::EventArgs& args)
	{
		_pendingRebuild = true;
	}

	void SimulationService::SetInputState(int keyCode, int state)
	{
		if (_IsRunning == true)
		{
			Application::Get().GetBehaviour().SetInputState(keyCode, state);
		}
	}

	void SimulationService::SetMousePosition(float x, float y) 
	{
		if (_IsRunning == true)
		{
			_MousePos[0] = x;
			_MousePos[1] = y;
		}
	}

	void SimulationService::SetMouseButtonState(int keyCode, bool state)
	{
		if (_IsRunning == true)
		{
			Application::Get().GetBehaviour().SetMouseButtonState(keyCode, state);
		}
	}


	void SimulationService::CompileAll()
	{
		/* clear all the code logs from the output */
		_EventService->Publish(Events::EventType::ClearLogEntry, std::make_unique<Events::ClearLogEntryArgs>(LogType::Code));

		std::vector<std::string> scriptFiles;
		for (auto& [key, value] : _ResourceService->GetResources())
		{
			auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
			if (downcastedPtr)
			{
				scriptFiles.push_back(value->GetPath());
			}
		}
		
		/**
		* First stage is to compile all the file into a loadable library.
		* Note: we want to compile this into the temp directory, incase the compilation fails
		*/
		const std::string tempModelFileName = Utils::GetTempFolder() + "\\" + _WorkspaceService->GetCurrentProject()->name + ".dll";
		const std::string finalModelFileName = Utils::GetPath(_WorkspaceService->GetLoadedProjectPath()) + "\\" + _WorkspaceService->GetCurrentProject()->name + ".dll";

		CompileResults results;
		Application::Get().GetBehaviour().CompileAll(scriptFiles, tempModelFileName, results);

		/* if we not successful we want to format some messages and send them to the editor */
		if (!results.success)
		{
			for (auto& err : results.errors)
			{
				_EventService->Publish(Events::EventType::AddLogEntry, std::make_unique<Events::AddLogEntryArgs>(LogType::Code, Severity::Error, err));
			}
		}
		else
		{ 
			/* Second stage is to copy of the successfully compiled model to the execution director, only if the compilation was successful */
			Utils::RemoveFile(finalModelFileName);
			Utils::CopySingleFile(tempModelFileName, Utils::GetPath(_WorkspaceService->GetLoadedProjectPath()) + "\\");
		}

		/* Third stage is to load the model into the scripting environment if we have valid model */
		if (Utils::FileExists(finalModelFileName) == true)
		{
			Application::Get().GetBehaviour().LoadBehaviourModel(scriptFiles, finalModelFileName);
		}

		/* Final state, if we have an open scene we need to reassign the scripts, as the class pointers will most likely have changed */
		if (_WorkspaceService->GetLoadedScene() != nullptr)
		{
			_WorkspaceService->GetLoadedScene()->AssignScripts(Application::Get().GetBehaviourPtr());
		}
	}

	std::shared_ptr<Osiris::ScriptedClass> SimulationService::GetClass(const std::string& className)
	{
		return Application::Get().GetBehaviour().GetCustomClass(className);
	}


}