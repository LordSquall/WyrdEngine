/* local includes */
#include "wyrdpch.h"
#include "Application.h"
#include "core/Log.h"
#include "core/Window.h"
#include "core/Resources.h"
#include "core/behaviour/Behaviour.h"
#include "core/renderer/Renderer.h"
#include "core/physics/Physics.h"
#include "core/LayerStack.h"
#include "events/ApplicationEvent.h"
#include "layers/BehaviourLayer.h"
#include "layers/PhysicsLayer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Texture.h"

/* external includes */
#include <GLFW/glfw3.h>
#include <SOIL.h>

namespace Wyrd {

	Application* Application::s_Instance = nullptr;

	Application::Application(const AppProps& props)
	{
		/* save the local instance of the application */
		s_Instance = this;

		/* store the thread ID */
		_MainThreadID = std::this_thread::get_id();

		/* output dir and pid */
		WYRD_CORE_INFO("cwd : {0} on PID : {1}", std::filesystem::current_path(), _MainThreadID);

		/* call the pre init function */
		//OnPreAppCreation(this);

		_LayerStack = std::make_unique<LayerStack>();

		/* create a windows and bind the event callback */
		_Window = std::unique_ptr<Window>(Window::Create(props.windowProps));
		_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		_Window->SetSize(props.windowProps.Width, props.windowProps.Height);
		
		/* create a renderer */
		_Renderer.reset(Renderer::Create());

		/* create the resource subsystem */
		_Resources = std::make_unique<Resources>();

		{
			/* default sprite texture */
			int width, height, channels;
			unsigned char* data;

			data = SOIL_load_image("../../Wyrd/res/textures/box_01.png", &width, &height, &channels, 4);

			TextureDesc textureDesc;
			textureDesc.data = data;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.channels = channels;
			textureDesc.description = "Default Sprite Texture";

			std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(Texture::Create(textureDesc));
			texture->SetUID(UID(RESOURCE_DEFAULT_TEXTURE));
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
		color[0] = 1.0f;
		color[1] = 0.2f;
		color[2] = 0.2f;
	}

	Application::~Application()
	{
		WYRD_TRACE("Application::Close()");
	}

	bool Application::PushLayer(Layer* layer)
	{
		return _LayerStack->PushLayer(layer);
	}

	bool Application::PushOverlay(Layer* overlay)
	{	
		return _LayerStack->PushOverlay(overlay);
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
			for (auto it = _LayerStack->end(); it != _LayerStack->begin(); )
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
			Frame(nullptr, 0.0f, 0.0f, 0.0f);
			_Running = !_Window->GetCloseRequested();
		}

		/* detach each of the layers */
		for (Layer* layer : *_LayerStack)
		{
			layer->OnDetach();
		}
	}

	void Application::Frame(char* outputBuffer, float r, float g, float b)
	{
		float time = (float)glfwGetTime();			// should make this platform independant
		Timestep timestep = time - _LastFrameTime;
		_LastFrameTime = time;

		_Window->OnUpdate();

		/* run the layer and window lifecycle */
		for (Layer* layer : *_LayerStack)
		{
			if (layer->IsEnabled())
				layer->OnUpdate(timestep);
		}

		/* clear the back buffer */
		_Renderer->Clear(r, g, b);

		_Window->OnPreRender();
		_Window->OnRender();

		for (Layer* layer : *_LayerStack)
		{
			if (layer->IsEnabled())
				layer->OnRender(timestep, *_Renderer);
		}

		_Window->OnPostRender();

		if (outputBuffer != nullptr)
			_Renderer->CopyPixels(0, 0, _Window->GetWidth(), _Window->GetHeight(), (BYTE*)outputBuffer);
	}

	void Application::Resize(int width, int height)
	{
		_Window->SetSize(width, height);
	}
}