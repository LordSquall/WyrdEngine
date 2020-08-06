#pragma once

#include "core/Window.h"

#include "core/renderer/GraphicsContext.h"

#include "events/ApplicationEvent.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Osiris {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		
		void OnRender() override;
		void OnPreRender() override; 
		void OnPostRender() override;
		
		void OnEvent(Event& e) override;
		
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void SetSize(int Width, int height) override;

		inline int GetX() const override { 
			return m_Data.X; 
		}
		inline int GetY() const override { return m_Data.Y; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetTitle(const std::string& title) override;

		void SetCloseRequested(bool close) override;
		bool GetCloseRequested() const override;

		void* GetNativeWindowPointer() const override;
		void* GetNativeGFXContextPointer() const override;
		
	private:
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnWindowCloseEvent(WindowCloseEvent& e);

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;

		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			int X, Y;
			bool VSync;
			double CursorX, CursorY;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		bool m_CloseRequested;
	};
}