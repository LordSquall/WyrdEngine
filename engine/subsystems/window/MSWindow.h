#ifndef _MS_WINDOW_H_
#define _MS_WINDOW_H_

#include "export.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IWindow.h"

#include <windows.h>
#include <windowsx.h>
#include <thread>
#include <string>

#include <GL\glew.h>
#include <GL\wglew.h>

namespace OrisisEngine
{
	struct MSWindow_Params
	{
		WNDCLASSEX wc;
		RECT wr;
		FrameworkConfig config;
	};

	class MSWindow : public IWindow {
		public:
			MSWindow(HINSTANCE hInstance);
			MSWindow(const MSWindow &obj);
			~MSWindow();

			void RegisterLogger(ILogger* logger);
			ILogger* GetLogger();

			void* GetPlatformDC();

			bool Create(FrameworkConfig* config);
			void Shutdown();
			bool IsRunning();

			void PresentFrame();

	private:
			bool WindowsEventLoop(MSWindow_Params* params);
			static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			HWND		_Handle;
			HINSTANCE	_Instance;
			HDC			_DeviceContext;
			HGLRC		_RenderingContext;
			ILogger*	_logger;

			string		_Title;

			static bool _IsRunning;

			bool	_Complete;

			thread _EventThread;

			struct MSWindow_Params _Params;
	};
}

#endif