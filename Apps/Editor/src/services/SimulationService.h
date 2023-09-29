#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/behaviour/Behaviour.h>

/* local includes */
#include "services/ServiceTypes.h"
#include "services/IService.h"
#include "events/EditorEvents.h"
#include "managers/EditorSceneManager.h"

namespace Wyrd::Editor
{
	class EventService;
	class ResourceService;
	class WorkspaceService;

	class SimulationService : public IService
	{
	public:
		SimulationService() : _IsRunning(false), _IsPaused(false), _IsAvailable(true), _pendingRebuild(false) {}
		~SimulationService() {}

	public:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate() override;

		static ServiceType GetTypeID() { return ServiceType::Simulation; }

	public:
		void Start();
		void Stop();
		void TogglePause();
		void Update(Timestep ts);

		void OnSceneOpenedEvent(Events::EventArgs& args);
		void OnBuildBehaviourModelEvent(Events::EventArgs& args);
		void OnBuildBehaviourBuiltEvent(Events::EventArgs& args);

		inline bool IsRunning() const { return _IsRunning; }
		inline bool IsAvailable() const { return _IsAvailable; }

		inline void SetSceneViewer(std::shared_ptr<EditorViewBase> sceneViewer) { _SceneViewer = sceneViewer; }
		void SetInputState(int keyCode, int state);
		void SetMousePosition(float x, float y);
		void SetMouseButtonState(int i, bool state);
			
		bool CompileAll();

		std::shared_ptr<Wyrd::ScriptedClass> GetClass(const std::string& className);

	private:
		bool _IsRunning;
		bool _IsPaused;
		bool _IsAvailable;
		bool _pendingRebuild;
		bool _delayedRebuild;

		float _MousePos[2];

		EditorSceneManager _SceneManager;

		std::shared_ptr<EditorViewBase>		_SceneViewer;
		std::shared_ptr<Scene>				_CurrentScene;
		std::shared_ptr<EventService>		_EventService;
		std::shared_ptr<ResourceService>	_ResourceService;
		std::shared_ptr<WorkspaceService>	_WorkspaceService;
	};
}