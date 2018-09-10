#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

/* Version info string */
#define ORS_FW_VER_INFO "Version 0.0.1 [In Development]"

/* System header files */
#include <Windows.h>
#include <map>

/* Project header file*/
#include "export.h"
#include "config.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IWindow.h"
#include "subsystems\interfaces\IRenderer.h"
#include "subsystems\sprites\SpriteSystem.h"

#include "subsystems\graphics\Scene.h"

namespace OrisisEngine
{
	class DllExport Framework: public ISubSystem{

		public:
			/* Default Constructor */
			Framework();

			/* Copy Constructor */
			Framework(const Framework &obj);
			
			/* Deconstructor */
			~Framework();

			/**
			* Initailise the framework. This function creates each of the internal subsystems.
			* @param hInstance handle to the windows instance 
			* @param Path configFilePath path to the configuration used for the framework
			* @return 0 if successful
			*/
			int Initialise(HINSTANCE hInstance, string configFilePath);

			/**
			* Shutdown the framework. This function will delete and terminate all subsystems within the framework
			*/
			void Shutdown();

			/**
			* Return a human readable string detailing the version info for the framework 
			* @return version info
			*/
			string GetVersionInfo();

			bool Run();

			bool ExecuteFrame();

			void RegisterLogger(ILogger* logger);
			ILogger* GetLogger();
			
			void AddScene(string name, Scene* scene);
			void SetCurrentScene(string name);

			SpriteSystem*	GetSpriteSystem();

		private:
			map<string, ISubSystem*> _subSystems;

			ILogger* _logger;

			IWindow* _window;
			IRenderer* _renderer;

			SpriteSystem _spriteSystem;

			map<string, Scene*> _scenes;

			Scene* _currentScene;
	};
}

#endif