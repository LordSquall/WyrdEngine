#pragma once

#include "osrpch.h"
#include "export.h"
#include "events/Event.h"

namespace Osiris
{
	struct WindowProps
	{
		std::string Title;
		int Width;
		int Height;
		int X;
		int Y;

		WindowProps(const std::string& title = "Osiris Engine",
			int width  = 1280,
			int height = 720,
			int x = 50,
			int y = 75)
			: Title(title), Width(width), Height(height), X(x), Y(y)
		{
		}
	};

	class OSR_LIBRARY_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual void OnRender() = 0;
		virtual void OnPreRender() = 0;
		virtual void OnPostRender() = 0;

		virtual void OnEvent(Event& e) = 0;
		
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetSize(int width, int height) = 0;

		virtual int GetX() const = 0;
		virtual int GetY() const = 0;

		virtual void SetSize(int width, int height) = 0;

		inline float GetAspectRatio() const { return (float)GetWidth() / (float)GetHeight(); }

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void SetCloseRequested(bool close) = 0;
		virtual bool GetCloseRequested() const = 0;

		virtual void* GetNativeWindowPointer() const = 0;
		virtual void* GetNativeGFXContextPointer() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}