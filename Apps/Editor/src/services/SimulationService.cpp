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
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::BuildBehaviourModel, EVENT_FUNC(SimulationService::OnBuildBehaviourModelEvent));

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


	void SimulationService::CompileAll()
	{
		/* clear all the code logs from the output */
		_EventService->Publish(Events::EventType::ClearLogEntry, std::make_shared<Events::ClearLogEntryArgs>(LogType::Code));

		std::vector<std::string> scriptFiles;
		for (auto& [key, value] : _ResourceService->GetResources())
		{
			auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
			if (downcastedPtr)
			{
				scriptFiles.push_back(value->GetPath());
			}
		}

		CompileResults results;
		Application::Get().GetBehaviour().CompileAll(scriptFiles, Utils::GetPath(_WorkspaceService->GetLoadedProjectPath()), _WorkspaceService->GetCurrentProject()->name + ".dll", results);

		if (!results.success)
		{
			for (auto& err : results.errors)
			{
				_EventService->Publish(Events::EventType::AddLogEntry, std::make_shared<Events::AddLogEntryArgs>(LogType::Code, Severity::Error, err));
			}

			return;
		}

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