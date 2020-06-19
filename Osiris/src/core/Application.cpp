#include "osrpch.h"

#include "Application.h"
#include "events/ApplicationEvent.h"

#include "GLFW/glfw3.h"

namespace Osiris {

	Application* Application::s_Instance = nullptr;

	Application::Application(const AppProps& props)
	{
		/* save the local instance of the application */
		s_Instance = this;

		/* call the pre init function */
		OnPreAppCreation(this);

		/* create a windows and bind the event callback */
		m_Window = std::unique_ptr<Window>(Window::Create(props.windowProps));
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

	bool Application::PushLayer(Layer* layer)
	{
		return m_LayerStack.PushLayer(layer);
	}

	bool Application::PushOverlay(Layer* overlay)
	{	
		return m_LayerStack.PushOverlay(overlay);
	}


	void Application::Close()
	{
		OnEvent(WindowCloseEvent());
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		/* send the event of the window(s) first */
		m_Window->OnEvent(e);

		if (e.Handled == false)
		{
			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
			{
				(*--it)->OnEvent(e);
				if (e.Handled)
					break;
			}
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();			// should make this platform independant
			Timestep timestep = time - _LastFrameTime;
			_LastFrameTime = time;

			m_Window->OnUpdate();

			/* run the layer and window lifecycle */
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsEnabled())
					layer->OnUpdate(timestep);
			}

			/* clear the back buffer */
			m_Renderer->Clear(color[0], color[1], color[2]);

			m_Window->OnPreRender();
			m_Window->OnRender();

			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsEnabled())
					layer->OnRender(timestep , *m_Renderer);
			}

			m_Window->OnPostRender();

			m_Running = !m_Window->GetCloseRequested();
		}

		/* detach each of the layers */
		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
		}
	}
}