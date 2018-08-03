#include "MSWindow.h"

using namespace OrisisEngine;

bool MSWindow::_IsRunning = true;

MSWindow::MSWindow(HINSTANCE hInstance)
{
	_Instance = hInstance;
	_Complete = false;
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

void* MSWindow::GetPlatformDC()
{
	return _DeviceContext;
}

bool MSWindow::Create(FrameworkConfig* config)
{
	_logger->LogInfo("MSWindow: Creating Window");

	_Title = config->title;

	_Params.wr = { 0, 0, config->width, config->height };
	memcpy(&_Params.config, config, sizeof(FrameworkConfig));

	// clear out the window class for use
	ZeroMemory(&_Params.wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	_Params.wc.cbSize = sizeof(_Params.wc);
	_Params.wc.style = CS_HREDRAW | CS_VREDRAW;
	_Params.wc.lpfnWndProc = this->WndProc;
	_Params.wc.hInstance = _Instance;
	_Params.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	_Params.wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	_Params.wc.lpszClassName = "Core";

	_EventThread = thread(&MSWindow::WindowsEventLoop, this, &_Params);

	_IsRunning = true;

	while (_Complete == false)
	{
		/* Wait until creation thread had completed loading */
	}

	if (!wglMakeCurrent(_DeviceContext, _RenderingContext)) {
		_logger->LogError("wglMakeCurrent() failed.");
		return false;
	}

	_Title.append(" - ").append((LPCSTR)glGetString(GL_VERSION));
	SetWindowText(_Handle, _Title.c_str());
	ShowWindow(_Handle, true);

	GLint glStatus = GL_NO_ERROR;
	glStatus = glGetError();

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

void MSWindow::PresentFrame()
{
	SwapBuffers(_DeviceContext);
}

bool MSWindow::WindowsEventLoop(MSWindow_Params* params)
{
	HWND tempWindow;
	HDC tempDeviceContext;
	PIXELFORMATDESCRIPTOR tempPFD;
	int tempPFDId;
	HGLRC tempRenderingContext;
	MSG msg;
	
	RegisterClassEx(&params->wc);

	AdjustWindowRect(&params->wr, WS_OVERLAPPEDWINDOW, FALSE); 
	
	/* Create a temporary window */
	tempWindow = CreateWindowEx(NULL,
		"Core", "Fake Window",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		1, 1, 
		NULL, NULL, 
		_Instance, NULL);

	/* Get temporary device context */
	tempDeviceContext = GetDC(tempWindow);

	/* Initialise a temporary pixel description */
	ZeroMemory(&tempPFD, sizeof(tempPFD));
	tempPFD.nSize = sizeof(tempPFD);
	tempPFD.nVersion = 1;
	tempPFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	tempPFD.iPixelType = PFD_TYPE_RGBA;
	tempPFD.cColorBits = 32;
	tempPFD.cAlphaBits = 8;
	tempPFD.cDepthBits = 24;

	tempPFDId = ChoosePixelFormat(tempDeviceContext, &tempPFD);
	if (tempPFDId == 0) {
		_logger->LogError("ChoosePixelFormat() failed.");
		return false;
	}

	if (SetPixelFormat(tempDeviceContext, tempPFDId, &tempPFD) == false) {
		_logger->LogError("SetPixelFormat() failed.");
		return false;
	}

	tempRenderingContext = wglCreateContext(tempDeviceContext);
	
	if (tempRenderingContext == 0) {
		_logger->LogError("wglCreateContext() failed.");
		return false;
	}

	if (wglMakeCurrent(tempDeviceContext, tempRenderingContext) == false) {
		_logger->LogError("wglMakeCurrent() failed.");
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		_logger->LogError("glewInit() failed.");
		return false;
	}

	_Handle = CreateWindowEx( NULL,
		"Core", _Params.config.title,
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		_Params.config.x, _Params.config.y,  
		_Params.config.width, _Params.config.height,
		NULL, NULL,
		_Instance, NULL);

	_DeviceContext = GetDC(_Handle);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pixelFormatID; 
	UINT numFormats;

	bool status = wglChoosePixelFormatARB(_DeviceContext, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) {
		_logger->LogError("wglChoosePixelFormatARB() failed.");
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(_DeviceContext, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(_DeviceContext, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 5;
	int  contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	_RenderingContext = wglCreateContextAttribsARB(_DeviceContext, 0, contextAttribs);
	if (_RenderingContext == NULL) {
		_logger->LogError("wglCreateContextAttribsARB() failed.");
		return false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempRenderingContext);
	ReleaseDC(tempWindow, tempDeviceContext);
	DestroyWindow(tempWindow);

	_Complete = true;

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
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	// Handle any messages the switch statement didn't
	return 0;
}