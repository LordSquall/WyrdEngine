#include "osrpch.h"

#include "Application.h"
#include "events/ApplicationEvent.h"

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

		/* load and build the sprite shader */
		std::ifstream vertexStream("../../Osiris/res/shaders/sprite.vs");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("../../Osiris/res/shaders/sprite.fs");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_Shader.reset(Shader::Create());
		_Shader->Build(vertexSrc, fragmentSrc);
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
			/* bind the sprite shader */
			_Shader->Bind();

			/* clear the back buffer */
			m_Renderer->Clear(0.1f, 0.1f, 0.1f);

			/* run the layer and window lifecycle */
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
			m_Window->OnPreRender();
			m_Window->OnRender();

			for (Layer* layer : m_LayerStack)
				layer->OnRender(*m_Renderer);

			m_Window->OnPostRender();
		}
	}
}