#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#define ORS_FW_VER_INFO "Version 0.0.1 [In Development]"

#include <Windows.h>

#include "export.h"
#include "config.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IWindow.h"
#include "subsystems\interfaces\IRenderer.h"

namespace OrisisEngine
{
	class DllExport Framework: public ISubSystem{

		public:
			Framework();
			Framework(const Framework &obj);
			~Framework();

			int Initialise(HINSTANCE hInstance, string configFilePath);
			int Shutdown();

			string GetVersionInfo();

			bool Run();

			bool ExecuteFrame();

			void RegisterLogger(ILogger* logger);

		private:
			ILogger* _logger;

			IWindow* _window;
			IRenderer* _renderer;
	};
}

#endif