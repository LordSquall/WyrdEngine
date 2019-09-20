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
		ServiceManager::Get<EventService>(ServiceManager::Events).Subscribe(Events::EventType::CreateNewProject, [this](Events::EventArgs& args) {
			Events::CreateNewProjectArgs& a = (Events::CreateNewProjectArgs&)args;
			CreateNewProject(a.location + "\\" + a.name + ".oproj");

		});

		ServiceManager::Get<EventService>(ServiceManager::Events).Subscribe(Events::EventType::OpenProject, [this](Events::EventArgs& args) {

		});

		ServiceManager::Get<EventService>(ServiceManager::Events).Subscribe(Events::EventType::CloseProject, [this](Events::EventArgs& args) {

		});

	}

	void Osiris::Editor::ProjectService::OnDestroy()
	{

	}

	void Osiris::Editor::ProjectService::CreateNewProject(std::string projectpath)
	{
		json j;
		j["name"] = "test";

		std::ofstream o(projectpath.c_str());

		o << std::setw(4) << j << std::endl;

		o.close();

		/* Create default folders and files */
		Utils::CreateProjectFileStructure(Utils::GetPath(projectpath));
	}


	void Osiris::Editor::ProjectService::LoadProject(std::string projectfile)
	{
		std::ifstream i(projectfile.c_str());
		json j;
		i >> j;

		_Project = std::make_shared<Project>();

		ProjectLoader::Load(projectfile.c_str(), *_Project, FileContent::Json);

		Utils::SetRootProjectFolder(Utils::GetPath(projectfile.c_str()));

		std::string initialScene = j["initialScene"];

		std::string projectPath = Utils::GetPath(projectfile);
		
		ServiceManager::Get<EventService>(ServiceManager::Events).Publish(Events::EventType::ProjectLoaded, Events::ProjectLoadedArgs(initialScene, _Project, projectPath));
	}
}
