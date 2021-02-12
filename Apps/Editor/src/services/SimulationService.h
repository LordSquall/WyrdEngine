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
		SimulationService() : _IsRunning(false), _pendingRebuild(false){}
		~SimulationService() {}

	public:
		// Inherited via IService
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate() override;

	public:
		void Start();
		void Stop();
		void Update(Timestep ts);

		void OnBuildBehaviourModelEvent(Events::EventArgs& args);

		inline bool IsRunning() const { return _IsRunning; }

		inline void SetSceneViewer(std::shared_ptr<EditorViewBase> sceneViewer) { _SceneViewer = sceneViewer; }
		void SetInputState(int keyCode, int state);
		void SetMousePosition(float x, float y);
			
		void CompileAll();

		std::shared_ptr<Osiris::ScriptedClass> GetClass(const std::string& className);

	private:
		bool _IsRunning;
		bool _pendingRebuild;

		float _MousePos[2];

		std::shared_ptr<EditorViewBase>		_SceneViewer;
		std::shared_ptr<Scene>				_CurrentScene;
		std::shared_ptr<EventService>		_EventService;
		std::shared_ptr<ResourceService>	_ResourceService;
		std::shared_ptr<WorkspaceService>	_WorkspaceService;
	};
}