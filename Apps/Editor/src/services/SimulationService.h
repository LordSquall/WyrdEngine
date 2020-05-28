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

	private:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		void Start();
		void Stop();
		void Update(Timestep ts);

		bool RunScript(const std::string& src);
		bool RunScriptFile(const std::string& path);

		std::shared_ptr<ScriptedObjectTemplate> CreateScriptableObjectTemplate(const std::string& source);

	private:
		static void DebugPrintFunc(const std::string& s);

	private:
		bool _IsRunning;

		std::shared_ptr<Scene> _CurrentScene;

		std::shared_ptr<EventService> _EventService;
		std::shared_ptr<WorkspaceService> _WorkspaceService;
	};
}