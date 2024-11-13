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
		
		void SaveLayoutPreferences();
		void LoadLayoutPreferences();

	private:
		float _time;
		

		std::shared_ptr<EditorViewBase> _sceneViewer;
		std::map<std::string, std::shared_ptr<EditorViewBase>> _views;

		std::shared_ptr<EventService>		_Event;
		std::shared_ptr<WorkspaceService>	_Workspace;
		std::shared_ptr<SettingsService>	_Settings;
		std::shared_ptr<ResourceService>	_Resource;
		std::shared_ptr<SimulationService>	_Simulation;
		std::shared_ptr<DialogService>		_Dialog;

		std::shared_ptr<EditorViewBase> _mouseEventOwner;

		static bool _showSceneViewerContextMenu;
	};
}