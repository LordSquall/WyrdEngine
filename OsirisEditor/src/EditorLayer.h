#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "EditorPlugin.h"
#include "IconLibrary.h"

#include "tools/GameViewer.h"
#include "tools/SceneEditor.h"
#include "tools/SceneHierarchy.h"
#include "tools/ResourceViewer.h"
#include "tools/ProjectExplorer.h"

namespace Osiris::Editor
{
	class EditorLayer : public Layer
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