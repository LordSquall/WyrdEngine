#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>

/* local includes */
#include "SimulationService.h"
#include "services/ServiceManager.h"
#include "views/SceneViewer/SceneViewer.h"

/* external includes */
#include <glm/glm.hpp>
#include <iostream>

#define MONO_INSTALL_LOC "C:/PROGRA~1/Mono/"
#define NATIVE_API_LIB_LOC "C:/Projects/Wyrd/WyrdEngine/lib/Debug/"

namespace Wyrd::Editor
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
		WYRD_TRACE("Starting Simulation Service");

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

		WYRD_TRACE("Stopping Simulation Service");

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
		const std::string tempModelFileName = (_WorkspaceService->GetTempDirectory() / (_WorkspaceService->GetCurrentProject()->name + ".dll")).string();
		const std::string finalModelFileName = Utils::GetPath(_WorkspaceService->GetLoadedProjectPath()) + "\\" + _WorkspaceService->GetCurrentProject()->name + ".dll";

		// mono compiler script command
		std::string command = "mcs ";

		// add all the files in the project
		for (auto& file : scriptFiles)
		{
			command += file + " ";
		}

		std::string file_name = "error_output.txt";

		// delete the error file 
		std::remove(file_name.c_str());

		// add library and wyrd runtime 
		command += " -target:library -lib:" MONO_INSTALL_LOC  "lib/mono/4.5/Facades/," NATIVE_API_LIB_LOC "WyrdAPI/ -r:System.Runtime.InteropServices.dll,WyrdAPI.dll -debug ";

		// set the putput file
		command += "-out:" + tempModelFileName;

		// run the command to compile
		std::system((command + " 2> " + file_name).c_str()); // redirect output to file

		// get the size of the file to determine if there were any errors or warnings
		if (std::filesystem::file_size(file_name) > 0)
		{
			// read all the lines into a vector
			std::ifstream file(file_name);
			std::string str;;

			while (std::getline(file, str))
			{
				// Line contains string of length > 0 then save it in vector
				if (str.size() > 0)
				{
					_EventService->Publish(Events::EventType::AddLogEntry, std::make_unique<Events::AddLogEntryArgs>(LogType::Code, Severity::Error, str));
				}
			}


			// Mark the compiles as unsuccessful
			_IsAvailable = false;
		}
		else
		{
			/* Second stage is to copy of the successfully compiled model to the execution director, only if the compilation was successful */
			Utils::RemoveFile(finalModelFileName);
			Utils::CopySingleFile(tempModelFileName, Utils::GetPath(_WorkspaceService->GetLoadedProjectPath()) + "\\");

			_IsAvailable = true;
		}

		/* Third stage is to load the model into the scripting environment if we have valid model */
		if (Utils::FileExists(finalModelFileName) == true)
		{
			Application::Get().GetBehaviour().LoadBehaviourModel(scriptFiles, finalModelFileName);
		}

		_EventService->Publish(Events::EventType::RefreshView, std::make_unique<Events::RefreshViewEventArgs>());
	}

	std::shared_ptr<Wyrd::ScriptedClass> SimulationService::GetClass(const std::string& className)
	{
		return Application::Get().GetBehaviour().GetCustomClass(className);
	}


}