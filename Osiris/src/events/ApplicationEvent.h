#pragma once

#include "Event.h"

namespace Osiris
{
	class  WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	protected:

		unsigned int m_Width, m_Height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y)
			: m_X(x), m_Y(y) {}

		inline int GetX() const { return m_X; }
		inline int GetY() const { return m_Y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMoveEvent: " << m_X << ", " << m_Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMove)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	protected:

		int m_X, m_Y;
	};

	class  WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}


		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}


		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}


		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}