#include "osrpch.h"

#include "Application.h"
#include "events/ApplicationEvent.h"

#include "GLFW/glfw3.h"

#ifdef OSR_EDITOR_ENABLED
#include "editor/layers/EditorLayer.h"
#endif

namespace Osiris {
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		/* save the local instance of the application */
		s_Instance = this;

		/* create a windows and bind the event callback */
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));	

		/* create a renderer */
		m_Renderer.reset(Renderer::Create());

		/* set a default back buffer color */
		color[0] = 0.1f;
		color[1] = 0.1f;
		color[2] = 0.1f;
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{	
		m_LayerStack.PushOverlay(overlay);
	}


	void Application::OnEvent(Event& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();			// should make this platform independant
			Timestep timestep = time - _LastFrameTime;
			_LastFrameTime = time;

			/* clear the back buffer */
			m_Renderer->Clear(color[0], color[1], color[2]);

			/* run the layer and window lifecycle */
			for (Layer* layer : m_LayerStack)
			{
				if(layer->IsEnabled())
					layer->OnUpdate(timestep);
			}

			m_Window->OnUpdate();
			m_Window->OnPreRender();
			m_Window->OnRender();

			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsEnabled())
					layer->OnRender(timestep , *m_Renderer);
			}

			m_Window->OnPostRender();
		}
	}
}