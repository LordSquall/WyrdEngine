/* local includes */
#include "osrpch.h"
#include "Application.h"
#include "core/Log.h"
#include "events/ApplicationEvent.h"
#include "layers/BehaviourLayer.h"
#include "layers/PhysicsLayer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Texture.h"

/* external includes */
#include <GLFW/glfw3.h>
#include <SOIL.h>

namespace Osiris {

	Application* Application::s_Instance = nullptr;

	Application::Application(const AppProps& props)
	{
		/* save the local instance of the application */
		s_Instance = this;

		/* store the thread ID */
		_MainThreadID = std::this_thread::get_id();

		/* call the pre init function */
		OnPreAppCreation(this);

		/* create a windows and bind the event callback */
		_Window = std::unique_ptr<Window>(Window::Create(props.windowProps));
		_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		/* create a renderer */
		_Renderer.reset(Renderer::Create());

		/* create the resource subsystem */
		_Resources = std::make_unique<Resources>();

		/* add the inbuilt resources */
		{
			/* default sprite shader */
			std::ifstream vertexStream("../../Osiris/res/shaders/sprite.vs");
			std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

			std::ifstream fragmentStream("../../Osiris/res/shaders/sprite.fs");
			std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

			std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
			shader->Build(vertexSrc, fragmentSrc);
			shader->Bind();
			_Resources->Shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>("Sprite", shader));
		}
		{
			/* default sprite texture */
			int width, height, channels;
			unsigned char* data;

			data = SOIL_load_image("../../Osiris/res/textures/box_01.png", &width, &height, &channels, 4);

			std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::Create(data, width, height, channels, "Default Sprite Texture"));
			_Resources->Textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(UID(RESOURCE_DEFAULT_TEXTURE), texture));
		}
		
		/* create behaviour subsystem */
		_Behaviour = std::make_unique<Behaviour>();

		/* add engine layers */
		PushLayer(new BehaviourLayer());

		/* create physics subsystem */
		_Physics = std::make_unique<Physics>();

		/* add engine layers */
		PushLayer(new PhysicsLayer());

		/* set a default back buffer color */
		color.r = 0.1f;
		color.g = 0.1f;
		color.b = 0.1f;
	}

	Application::~Application()
	{
		OSR_TRACE("Application::Close()");
	}

	bool Application::PushLayer(Layer* layer)
	{
		return _LayerStack.PushLayer(layer);
	}

	bool Application::PushOverlay(Layer* overlay)
	{	
		return _LayerStack.PushOverlay(overlay);
	}


	void Application::Close()
	{
		//OnEvent(WindowCloseEvent());
		_Running = false;
	}


	void Application::Terminate()
	{
		exit(0);
	}

	void Application::OnEvent(Event& e)
	{
		/* send the event of the window(s) first */
		_Window->OnEvent(e);

		if (e.Handled == false)
		{
			for (auto it = _LayerStack.end(); it != _LayerStack.begin(); )
			{
				(*--it)->OnEvent(e);
				if (e.Handled)
					break;
			}
		}
	}

	void Application::Run()
	{
		while (_Running)
		{
			float time = (float)glfwGetTime();			// should make this platform independant
			Timestep timestep = time - _LastFrameTime;
			_LastFrameTime = time;

			_Window->OnUpdate();

			/* run the layer and window lifecycle */
			for (Layer* layer : _LayerStack)
			{
				if (layer->IsEnabled())
					layer->OnUpdate(timestep);
			}

			/* clear the back buffer */
			_Renderer->Clear(color.r, color.g, color.b);

			_Window->OnPreRender();
			_Window->OnRender();

			for (Layer* layer : _LayerStack)
			{
				if (layer->IsEnabled())
					layer->OnRender(timestep , *_Renderer);
			}

			_Window->OnPostRender();

			_Running = !_Window->GetCloseRequested();
		}

		/* detach each of the layers */
		for (Layer* layer : _LayerStack)
		{
			layer->OnDetach();
		}
	}
}