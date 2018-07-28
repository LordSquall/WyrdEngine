#include "MSWindow.h"

using namespace OrisisEngine;

bool MSWindow::_IsRunning = true;

MSWindow::MSWindow(HINSTANCE hInstance)
{
	_Instance = hInstance;
}

MSWindow::MSWindow(const MSWindow &obj)
{
}

MSWindow::~MSWindow()
{
}

void MSWindow::RegisterLogger(ILogger* logger)
{
	_logger = logger;
	_logger->LogInfo("Logger Registered with MSWindow");
}

bool MSWindow::Create(FrameworkConfig* config)
{
	_logger->LogInfo("MSWindow: Creating Window");

	_Params.wr = { 0, 0, config->width, config->height };
	memcpy(&_Params.config, config, sizeof(FrameworkConfig));

	// clear out the window class for use
	ZeroMemory(&_Params.wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	_Params.wc.cbSize = sizeof(WNDCLASSEX);
	_Params.wc.style = CS_HREDRAW | CS_VREDRAW;
	_Params.wc.lpfnWndProc = this->WndProc;
	_Params.wc.hInstance = _Instance;
	_Params.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	_Params.wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	_Params.wc.lpszClassName = "WindowClass1";

	_EventThread = thread(&MSWindow::WindowsEventLoop, this, &_Params);

	_IsRunning = true;
	
	return true;
}

void MSWindow::Shutdown()
{
	/* Join the event thread */
	_EventThread.join();

	_logger->LogInfo("MSWindow: Shutting Down Window");
}

bool MSWindow::IsRunning()
{
	return _IsRunning;
}

bool MSWindow::WindowsEventLoop(MSWindow_Params* params)
{
	MSG msg;
	
	// register the window class
	RegisterClassEx(&params->wc);

	AdjustWindowRect(&params->wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

														  // create the window and use the result as the handle
	_logger->LogDebug(params->config.title);
	params->hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		params->config.title,   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		params->config.x,    // x-position of the window
		params->config.y,    // y-position of the window
		params->config.width,    // width of the window
		params->config.height,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		_Instance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(params->hWnd, true);

	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);

		// check to see if it's time to quit
		if (msg.message == WM_QUIT)
			break;
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

LRESULT MSWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (msg)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		_IsRunning = false;
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hwnd, msg, wParam, lParam);
}