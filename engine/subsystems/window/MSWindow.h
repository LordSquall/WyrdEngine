#ifndef _MS_WINDOW_H_
#define _MS_WINDOW_H_

#include "export.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IWindow.h"

#include <windows.h>
#include <windowsx.h>
#include <thread>

namespace OrisisEngine
{
	struct MSWindow_Params
	{
		HWND hWnd;
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
			bool Create(FrameworkConfig* config);
			void Shutdown();
			bool IsRunning();

	private:
			bool WindowsEventLoop(MSWindow_Params* params);
			static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			HINSTANCE _Instance;
			ILogger* _logger;

			static bool _IsRunning;

			thread _EventThread;

			struct MSWindow_Params _Params;
	};
}

#endif