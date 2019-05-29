#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "editor/EditorPlugin.h"
#include "editor/support/IconLibrary.h"

#include "editor/tools/GameViewer.h"
#include "editor/tools/SceneEditor.h"
#include "editor/tools/SceneHierarchy.h"
#include "editor/tools/ResourceViewer.h"
#include "editor/tools/ProjectExplorer.h"

namespace Osiris::Editor
{
	class OSIRIS_API EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnRender(std::shared_ptr<Renderer> renderer) override;
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

	private:
		float	m_Time;

		IconLibrary	m_IconLibrary;

		std::map<std::string, std::shared_ptr<EditorPlugin>> m_plugins;
	};
}