#pragma once

#include "osrpch.h"
#include "WindowsWindow.h"

#include "events/MouseEvent.h"
#include "events/KeyEvent.h"
#include "platform/OpenGL/OpenGLContext.h"

namespace Osiris {

	static bool s_GLFWInitialised = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		OSR_CORE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_CloseRequested = false;

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.X = props.X;
		m_Data.Y = props.Y;

		OSR_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialised)
		{
			int success = glfwInit();
			OSR_CORE_ASSERT(success, "Could not initialise GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialised = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwSetWindowPos(m_Window, m_Data.X, m_Data.Y);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.X = x;
			data.Y = y;

			WindowMoveEvent e(x, y);
			data.EventCallback(e);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(key);
			data.EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button, data.CursorX, data.CursorY);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button, data.CursorX, data.CursorY);
					data.EventCallback(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.CursorX = xPos; 
			data.CursorY = yPos;

			MouseMovedEvent e((float)xPos, (float)yPos);
			data.EventCallback(e);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	void WindowsWindow::OnRender()
	{

	}

	void WindowsWindow::OnPreRender()
	{

	}

	void WindowsWindow::OnPostRender()
	{
		glfwPollEvents();

		m_Context->SwapBuffer();
	}

	void WindowsWindow::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(OSR_BIND_EVENT_FN(WindowsWindow::OnWindowResizeEvent));
		dispatcher.Dispatch<WindowCloseEvent>(OSR_BIND_EVENT_FN(WindowsWindow::OnWindowCloseEvent));
	}

	bool WindowsWindow::OnWindowResizeEvent(WindowResizeEvent& e)
	{

		return true;
	}

	bool WindowsWindow::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		SetCloseRequested(true);

		return true;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}


	void WindowsWindow::SetTitle(const std::string& title)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);
		data.Title = title;

		glfwSetWindowTitle(m_Window, data.Title.c_str());
	}


	void WindowsWindow::SetCloseRequested(bool close)
	{
		m_CloseRequested = close;
	}

	bool WindowsWindow::GetCloseRequested() const
	{
		return m_CloseRequested;
	}

	void* WindowsWindow::GetNativeWindowPointer() const
	{
		return (void*)m_Window;
	}

	void* WindowsWindow::GetNativeGFXContextPointer() const
	{
		return (void*)glfwGetWGLContext(m_Window);
	}
}