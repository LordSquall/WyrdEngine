#include "osrpch.h"

#include "Application.h"
#include "events/ApplicationEvent.h"

#include "platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Osiris {
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OSR_CORE_ASSERT(!s_Instance, "Application is not null!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));	


		glGenVertexArrays(1, &_VertexArray);
		glBindVertexArray(_VertexArray);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		_vertexBuffer.reset(VertexBuffer::Create(&vertices[0], sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		unsigned int indices[3] = { 0, 1, 2 };

		_indexBuffer.reset(IndexBuffer::Create(&indices[0], sizeof(indices) / sizeof(uint32_t)));

		std::ifstream vertexStream("../../Osiris/res/shaders/example.vs");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("../../Osiris/res/shaders/example.fs");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_Shader.reset(new OpenGLShader());
		_Shader.get()->Build(vertexSrc, fragmentSrc);
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

			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			_Shader->Bind();
			glBindVertexArray(_VertexArray);
			glDrawElements(GL_TRIANGLES, _indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);


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