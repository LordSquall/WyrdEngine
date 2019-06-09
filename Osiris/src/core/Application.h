#pragma once

#include "export.h"
#include "Window.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "Resources.h"
#include "LayerStack.h"

#ifdef OSR_EDITOR_ENABLED
#define LAYERID_CORE_EDITORLAYER "EditorLayer"
#endif

namespace Osiris {

	class  Application
	{
	public:
		Application();
		~Application();

		void Run();

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Renderer& GetRenderer() { return *m_Renderer; }
		inline Window& GetWindow() { return *m_Window;  }
		inline Resources& GetResources() { return *m_Resources; }

		inline LayerStack* GetLayerStack() { return &m_LayerStack; }

	protected:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Resources> m_Resources;
		std::unique_ptr<Renderer> m_Renderer;

		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}