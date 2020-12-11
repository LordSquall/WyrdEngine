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

namespace Osiris::Editor
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
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

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

		std::shared_ptr<EditorViewBase> _mouseEventOwner;

		static bool _showSceneViewerContextMenu;
	};
}