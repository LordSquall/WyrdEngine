#pragma once

/* local includes */
#include "export.h"
#include "core/LayerStack.h"
#include "core/Timestep.h"
#include "core/Window.h"
#include "core/renderer/Renderer.h"
#include "core/Resources.h"
#include "core/behaviour/Behaviour.h"
#include "core/physics/Physics.h"

namespace Osiris {

	struct AppProps
	{
		WindowProps windowProps;
	};

	class OSR_LIBRARY_API Application
	{
	public:
		Application(const AppProps& props);
		virtual ~Application();
		
		void Run();

		virtual void OnEvent(Event& event);

		bool PushLayer(Layer* layer);
		bool PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }

		inline Renderer& GetRenderer() { return *_Renderer; }
		inline Window& GetWindow() { return *_Window;  }
		inline Resources& GetResources() { return *_Resources; }
		inline Behaviour& GetBehaviour() { return *_Behaviour; }
		inline Physics& GetPhysics() { return *_Physics; }

		inline LayerStack* GetLayerStack() { return &_LayerStack; }

		void Close();

		void Terminate();

		glm::vec3 color;
	protected:
		std::unique_ptr<Window>		_Window;
		std::unique_ptr<Resources>	_Resources;
		std::unique_ptr<Renderer>	_Renderer;
		std::unique_ptr<Behaviour>	_Behaviour;
		std::unique_ptr<Physics>	_Physics;

		float		_LastFrameTime = 0.0f;
		bool		_Running = true;
		LayerStack	_LayerStack;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

	void OnPreAppCreation(Application* app);
}