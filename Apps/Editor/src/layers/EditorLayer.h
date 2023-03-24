#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "views/EditorViewBase.h"
#include "support/IconLibrary.h"
#include "support/Utils.h"

#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;
		
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e, void* data);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e, void* data);
		bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);
		bool OnKeyTypedEvent(KeyTypedEvent& e, void* data);
		bool OnWindowResizeEvent(WindowResizeEvent& e, void* data);

		void OnSceneOpened(Events::EventArgs& args);
		
	private:
		float _time;
		

		std::shared_ptr<EditorViewBase> _sceneViewer;
		std::map<std::string, std::shared_ptr<EditorViewBase>> _views;

		std::shared_ptr<EventService>		_eventService;
		std::shared_ptr<WorkspaceService>	_workspaceService;
		std::shared_ptr<SettingsService>	_settingsService;
		std::shared_ptr<ResourceService>	_resourceService;
		std::shared_ptr<SimulationService>	_simulationService;
		std::shared_ptr<DialogService>		_dialogService;

		std::shared_ptr<Icon> _playButtonIcon;
		std::shared_ptr<Icon> _stopButtonIcon;
		std::shared_ptr<Icon> _pauseButtonIcon;

		std::shared_ptr<EditorViewBase> _mouseEventOwner;

		static bool _showSceneViewerContextMenu;
	};
}