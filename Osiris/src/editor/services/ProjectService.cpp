#pragma once

#include "osrpch.h"
#include "ServiceManager.h"
#include "ProjectService.h"
#include "editor/loaders/ProjectLoader.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

namespace Osiris::Editor
{
	void Osiris::Editor::ProjectService::OnCreate()
	{
		/* Initialise Defaults */
		IsProjectLoaded(false);

		/* Subscribe to Project lifecycle events */
		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CreateNewProject, [this](Events::EventArgs& args) {
			Events::CreateNewProjectArgs& a = (Events::CreateNewProjectArgs&)args;
			CreateNewProject(a.location + "\\" + a.name + ".oproj");
		});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::OpenProject, [this](Events::EventArgs& args) {});

		ServiceManager::Get<EventService>(ServiceManager::Events)->Subscribe(Events::EventType::CloseProject, [this](Events::EventArgs& args) {});
	}

	void Osiris::Editor::ProjectService::OnDestroy()
	{

	}

	void Osiris::Editor::ProjectService::CreateNewProject(std::string projectpath)
	{
		/* Create a new project shared pointer */
		_Project = std::make_shared<Project>();

		/* Save the project object to disk */
		ProjectLoader::Result result = ProjectLoader::Save(projectpath, *_Project, Osiris::FileContent::Json);
		
		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Create default folders and files */
			Utils::CreateProjectFileStructure(Utils::GetPath(projectpath));

			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(projectpath.c_str()));

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded,
				Events::ProjectLoadedArgs(_Project->initialScene, _Project, Utils::GetPath(projectpath)));
		}
	}


	void Osiris::Editor::ProjectService::LoadProject(std::string projectfile)
	{
		/* Create a new project shared pointer */
		_Project = std::make_shared<Project>();

		/* Load the project object */
		ProjectLoader::Result result = ProjectLoader::Load(projectfile.c_str(), *_Project, Osiris::FileContent::Json);

		if (result == ProjectLoader::Result::Success)
		{
			/* Mark project as loaded */
			IsProjectLoaded(true);

			/* Set the utilities to the base root folder */
			Utils::SetRootProjectFolder(Utils::GetPath(projectfile.c_str()));

			/* Send a Project Loaded Event */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::ProjectLoaded, 
				Events::ProjectLoadedArgs(_Project->initialScene, _Project, Utils::GetPath(projectfile)));
		}
	}
}
