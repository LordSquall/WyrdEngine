#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
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
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);

		_IsRunning = false;
	}

	void SimulationService::OnDestroy()
	{
	}

	void SimulationService::Start()
	{
		OSR_TRACE("Starting Simulation Service");

		_IsRunning = true;

		Application::Get().GetBehaviour().Start(_WorkspaceService->GetLoadedScene());
		Application::Get().GetPhysics().Start(_WorkspaceService->GetLoadedScene());
	}

	void SimulationService::Stop()
	{
		Application::Get().GetPhysics().Stop();
		Application::Get().GetBehaviour().Stop();

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
		}
	}

	void SimulationService::SetInputState(int keyCode, int state)
	{
		if (_IsRunning == true)
		{
			Application::Get().GetBehaviour().SetInputState(keyCode, state);
		}
	}

	Osiris::Behaviour::CreateCustomClassResult SimulationService::AddCustomScriptClass(const std::string& name, const std::string& filename)
	{
		/* use the behaviour subsystem to add a custom class */
		return Application::Get().GetBehaviour().AddScriptedClassFromFile(name, filename);
	}
}