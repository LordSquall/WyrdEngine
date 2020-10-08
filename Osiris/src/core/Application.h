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

	/**
	 * @brief Base application class
	 * 
	 * This it the base application class for all Osiris Applications.
	 * Provides access to each of the core subsystems and main loop functions
	*/
	class OSR_LIBRARY_API Application
	{
	public:
		/**
		 * @brief Constructor
		 * @param props - application properties
		*/
		Application(const AppProps& props);
		virtual ~Application();

		/**
		 * @brief Enters into the application main loop.
		 * Blocking Function
		*/
		void Run();

		/**
		 * @brief Called when an event is recieved
		 * @param event - event object
		*/
		virtual void OnEvent(Event& event);

		/**
		 * @brief Push a layer pointer on to the application layer stack
		 * @param layer - layer to be pushed
		 * @return true if layer was added
		*/
		bool PushLayer(Layer* layer);

		/**
		 * @brief Push a overlay pointer on to the application layer stack
		 * Overlays are added infront of all layers
		 * @param overlay 
		 * @return treu if overlay was added
		*/
		bool PushOverlay(Layer* overlay);

		/**
		 * @brief Retrieve application instance pointer
		 * @return application instance
		*/
		inline static Application& Get() { return *s_Instance; }

		/**
		 * @brief Get Renderer Subsystem
		 * @return - renderer subsystem object
		*/
		inline Renderer& GetRenderer() { return *_Renderer; }

		/**
		 * @brief Get Window Subsystem
		 * @return - window subsystem object
		*/
		inline Window& GetWindow() { return *_Window;  }
		
		/**
		 * @brief Get Resources Subsystem
		 * @return - resources subsystem object
		*/
		inline Resources& GetResources() { return *_Resources; }

		/**
		 * @brief Get Behaviour Subsystem
		 * @return - behaviour subsystem object
		*/
		inline Behaviour& GetBehaviour() { return *_Behaviour; }

		/**
		 * @brief Get Physics Subsystem
		 * @return - physics subsystem object
		*/
		inline Physics& GetPhysics() { return *_Physics; }

		/**
		 * @brief Get LayerStack Subsystem
		 * @return - layerstack subsystem object
		*/
		inline LayerStack* GetLayerStack() { return &_LayerStack; }

		/**
		 * @brief Close the application. 
		 * This function will spawn a 'Close' event and propogate throught the application layers
		*/
		void Close();

		/**
		 * @brief Terminate the application process. e.g. Quit()
		*/
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


	/**
	 * @brief External Create Application function. This should be supplied by the Client application
	 * @return new client application pointer
	*/
	Application* CreateApplication();

	/**
	 * @brief Called after subsystem are created, but before intialisation
	 * This should be supplied by the Client application
	 * @param app - client application pointer
	*/
	void OnPreAppCreation(Application* app);
}