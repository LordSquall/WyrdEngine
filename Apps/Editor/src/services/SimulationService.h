#pragma once

#include "core/export.h"

#include "services/IService.h"

#include "behaviour/ScriptedObjectTemplate.h"

#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class EventService;
	class WorkspaceService;

	class SimulationService : public IService
	{
	public:
		SimulationService() : _IsRunning(false) {}
		~SimulationService() {}

	public:
		struct RunReturnVal {
			bool result;
			std::string error;
			std::shared_ptr<ScriptedObjectTemplate> scriptedObjectTemplate;
		};

		struct CreateReturnVal {
			bool result;
			std::string error;
			std::shared_ptr<ScriptedObjectTemplate> scriptedObjectTemplate;
		};

	private:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		void Start();
		void Stop();
		void Update(Timestep ts);

		RunReturnVal RunScript(const std::string& src);
		RunReturnVal RunScriptFile(const std::string& path);

		CreateReturnVal CreateScriptableObjectTemplate(const std::string& source);

	private:
		static void DebugPrintFunc(const std::string& s);

	private:
		bool _IsRunning;

		std::shared_ptr<Scene> _CurrentScene;

		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
	};
}