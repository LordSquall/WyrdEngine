#pragma once

#include "core/export.h"

#include "editor/datamodels/Project.h"

namespace Osiris::Editor
{
	class ProjectService : public IService
	{
	public:
		ProjectService() {}
		~ProjectService() {}

		// Inherited via IService
		virtual void OnCreate() override;

		virtual void OnDestroy() override;

		void CreateNewProject(std::string foldername);
		void LoadProject(std::string projectfile);

		inline std::shared_ptr<Project> GetCurrentProject() { return _Project; }

	private:
		std::shared_ptr<Project> _Project;
	};
}