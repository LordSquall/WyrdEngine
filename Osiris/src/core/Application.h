#pragma once

#include "export.h"
#include "Window.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Buffer.h"
#include "Resources.h"
#include "LayerStack.h"

namespace Osiris {

	class  Application
	{
	public:
		Application();
		~Application();

		void Run();

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Renderer& GetRenderer() { return *m_Renderer; }
		inline Window& GetWindow() { return *m_Window;  }
		inline Resources& GetResources() { return *m_Resources; }
	protected:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Resources> m_Resources;
		std::shared_ptr<Renderer> m_Renderer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int _VertexArray;
		std::unique_ptr<Shader> _Shader;
		std::unique_ptr<VertexBuffer> _vertexBuffer;
		std::unique_ptr<IndexBuffer> _indexBuffer;


	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}