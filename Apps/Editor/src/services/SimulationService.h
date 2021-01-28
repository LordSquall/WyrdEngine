#pragma once

/* core osiris includes */
#include <core/export.h>
#include <core/behaviour/Behaviour.h>

/* local includes */
#include "services/IService.h"
#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class EventService;
	class ResourceService;
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

		inline bool IsRunning() const { return _IsRunning; }

		void SetInputState(int keyCode, int state);

		void CompileAll();

		std::shared_ptr<Osiris::ScriptedClass> GetClass(const std::string& className);

	private:
		bool _IsRunning;

		std::shared_ptr<Scene>				_CurrentScene;
		std::shared_ptr<EventService>		_EventService;
		std::shared_ptr<ResourceService>	_ResourceService;
		std::shared_ptr<WorkspaceService>	_WorkspaceService;
	};
}