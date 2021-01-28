#pragma once

#include "osrpch.h"
#include "export.h"
#include "events/Event.h"

namespace Osiris
{
	/**
	 * @brief Window Properties
	*/
	struct WindowProps
	{
		/**
		 * @brief Primary window Title
		*/
		std::string Title;

		/**
		 * @brief Window width in pixels
		*/
		int Width;

		/**
		 * @brief Window height in pixels
		*/
		int Height;

		/**
		 * @brief Window x position in pixels
		*/
		int X;

		/**
		 * @brief Window y position in pixels
		*/
		int Y;

		/**
		 * @brief Constructor
		 * @param title 
		 * @param width 
		 * @param height 
		 * @param x 
		 * @param y 
		*/
		WindowProps(const std::string& title = "Osiris Engine",
			int width  = 1280,
			int height = 720,
			int x = 50,
			int y = 75)
			: Title(title), Width(width), Height(height), X(x), Y(y)
		{
		}
	};

	/**
	 * @brief Window SubSystem
	 * This class provides a abstract interface for a Operating system window.
	 * Also this class offers a simple set of functions to manipulate the window state, as well
	 * as a number of overrideable functions to supplied windows with callbacks and events
	*/
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