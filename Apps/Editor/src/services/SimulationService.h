#pragma once

/* core osiris includes */
#include <core/export.h>

/* local includes */
#include "services/IService.h"
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

		void SetInputState(int keyCode, int state);

		Osiris::Behaviour::CreateCustomClassResult AddCustomScriptClass(const std::string& name, const std::string& filename);

	private:
		bool _IsRunning;

		std::shared_ptr<Scene>				_CurrentScene;
		std::shared_ptr<EventService>		_EventService;
		std::shared_ptr<WorkspaceService>	_WorkspaceService;
	};
}