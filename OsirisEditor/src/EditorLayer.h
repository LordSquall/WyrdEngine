#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"
#include "IconLibrary.h"

#include "tools/GameViewer.h"
#include "tools/SceneEditor.h"
#include "tools/SceneHierarchy.h"
#include "tools/ResourceViewer.h"
#include "tools/ProjectExplorer.h"

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
		void UpdateMouse();
		void UpdateCursor();

	private:
		float	m_Time;

		IconLibrary	m_IconLibrary;

		std::map<std::string, std::shared_ptr<EditorPlugin>> m_plugins;
	};
}