#include "osrpch.h"

#include "Application.h"
#include "events/ApplicationEvent.h"

#include "graphics/Shader.h"

namespace Osiris {
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OSR_CORE_ASSERT(!s_Instance, "Application is not null!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));	

		m_Renderer = std::shared_ptr<Renderer>(Renderer::Create());
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
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
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();

			m_Window->OnPreRender();
			m_Window->OnRender();

			for (Layer* layer : m_LayerStack)
				layer->OnRender(m_Renderer);

			m_Window->OnPostRender();
		}
	}
}