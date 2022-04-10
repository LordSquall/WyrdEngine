#pragma once

/* local includes */
#include "export.h"
#include "core/Timestep.h"
#include "events/Event.h"
#include "Layer.h"

namespace Wyrd {

	class Window;
	class Renderer;
	class Resources;
	class Behaviour;
	class Physics;
	class LayerStack;


	/**
	 * @brief Window Properties
	*/
	struct WindowProps
	{
		/**
		 * @brief Primary window Title
		*/
		std::string Title;

		/**
		 * @brief Window width in pixels
		*/
		int Width;

		/**
		 * @brief Window height in pixels
		*/
		int Height;

		/**
		 * @brief Window x position in pixels
		*/
		int X;

		/**
		 * @brief Window y position in pixels
		*/
		int Y;

		bool isHidden;

		/**
		 * @brief Constructor
		 * @param title
		 * @param width
		 * @param height
		 * @param x
		 * @param y
		*/
		WindowProps(const std::string& title = "Wyrd Engine",
			int width = 1280,
			int height = 720,
			int x = 50,
			int y = 75,
			bool isHidden = false)
			: Title(title), Width(width), Height(height), X(x), Y(y), isHidden(isHidden)
		{
		}
	};

	struct AppProps
	{
		WindowProps windowProps;
	};

	/**
	 * @brief Base application class
	 * 
	 * This is the base application class for all Wyrd Applications.
	 * Provides access to each of the core subsystems and main loop functions
	*/
	class WYRD_LIBRARY_API Application
	{
	public:
		/**
		 * @brief Constructor
		 * @param props - application properties
		*/
		Application(const AppProps& props);
		virtual ~Application();

		/**
		 * @brief Create a new window and registers it with the application
		 * @param props - window properties
		*/
		Window* CreateNewWindow(const WindowProps& props);

		/**
		 * @brief Create a new renderer and registers it with the application
		*/
		Renderer* CreateNewRenderer();

		/**
		 * @brief Enters into the application main loop.
		 * Blocking Function
		*/
		void Run();

		/**
		 * @brief Perform a single frame
		 * Blocking Function
		*/
		void Frame(char* outputBuffer, float r, float g, float b);


		void Resize(int width, int height);

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
		 * @brief Get Raw Resources pointer
		 * @return - resources subsystem pointer
		*/
		inline Resources* GetResourcesPtr() { return _Resources.get(); }

		/**
		 * @brief Get Behaviour Subsystem
		 * @return - behaviour subsystem object
		*/
		inline Behaviour& GetBehaviour() { return *_Behaviour; }

		/**
		 * @brief Get Raw Behaviour pointer
		 * @return - behaviour subsystem pointer
		*/
		inline Behaviour* GetBehaviourPtr() { return _Behaviour.get(); }

		/**
		 * @brief Get Physics Subsystem
		 * @return - physics subsystem object
		*/
		inline Physics& GetPhysics() { return *_Physics; }

		/**
		 * @brief Get Raw Physics pointer
		 * @return - physics subsystem pointer
		*/
		inline Physics* GetPhysicsPtr() { return _Physics.get(); }

		/**
		 * @brief Get LayerStack Subsystem
		 * @return - layerstack subsystem object
		*/
		inline LayerStack* GetLayerStack() { return _LayerStack.get(); }

		/**
		 * @brief Close the application. 
		 * This function will spawn a 'Close' event and propogate throught the application layers
		*/
		void Close();

		/**
		 * @brief Terminate the application process. e.g. Quit()
		*/
		void Terminate();

		/**
		 * @brief Get the main thread id
		 * @return - Main thread ID
		*/
		const std::thread::id& GetMainThreadID() { return _MainThreadID; }

		float color[3];

	protected:
		std::unique_ptr<Window>		_Window;
		std::unique_ptr<Resources>	_Resources;
		std::unique_ptr<Renderer>	_Renderer;
		std::unique_ptr<Behaviour>	_Behaviour;
		std::unique_ptr<Physics>	_Physics;

		float		_LastFrameTime = 0.0f;
		bool		_Running = true;
		std::unique_ptr<LayerStack>	_LayerStack;

		std::thread::id _MainThreadID;
	private:

		static Application* s_Instance;
	};


	/**
	 * @brief External Create Application function. This should be supplied by the Client application
	 * @return new client application pointer
	*/
	//Application* CreateApplication();

	/**
	 * @brief Called after subsystem are created, but before intialisation
	 * This should be supplied by the Client application
	 * @param app - client application pointer
	*/
	//void OnPreAppCreation(Application* app);
}