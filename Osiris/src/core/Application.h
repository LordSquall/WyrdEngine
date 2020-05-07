#pragma once

#include "export.h"
#include "Window.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "Resources.h"
#include "LayerStack.h"
#include "Timestep.h"

#ifdef OSR_EDITOR_ENABLED
#define LAYERID_CORE_EDITORLAYER "EditorLayer"
#endif

namespace Osiris {

	struct AppProps
	{
		WindowProps windowProps;
	};

	class Application
	{
	public:
		Application(const AppProps& props);
		virtual ~Application();
		
		void Run();

		virtual void OnEvent(Event& event);

		bool PushLayer(Layer* layer);
		bool PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Renderer& GetRenderer() { return *m_Renderer; }
		inline Window& GetWindow() { return *m_Window;  }
		inline Resources& GetResources() { return *m_Resources; }

		inline LayerStack* GetLayerStack() { return &m_LayerStack; }

		void Close();

		float color[3];
	protected:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Resources> m_Resources;
		std::unique_ptr<Renderer> m_Renderer;
		float _LastFrameTime = 0.0f;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

	void OnPreAppCreation(Application* app);
}