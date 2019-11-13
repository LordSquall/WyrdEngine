#pragma once

#include "osrpch.h"
#include "export.h"
#include "events/Event.h"

namespace Osiris
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Osiris Engine",
			unsigned int width  = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual void OnRender() = 0;
		virtual void OnPreRender() = 0;
		virtual void OnPostRender() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindowPointer() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}