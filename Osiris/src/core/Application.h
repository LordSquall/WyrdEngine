#pragma once

#include "export.h"
#include "Window.h"
#include "LayerStack.h"

namespace Osiris {

	class OSIRIS_API Application
	{
	public:
		Application();
		~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}