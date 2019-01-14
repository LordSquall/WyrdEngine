#pragma once

#include "Event.h"

namespace Osiris {

	enum KeyCode
	{
		TAB = 258,
		ARROW_LEFT = 263,
		ARROW_RIGHT = 262,
		ARROW_UP = 265,
		ARROW_DOWN = 264,
		PG_UP = 266,
		PG_DOWN = 267,
		FUNC_HOME = 268,
		FUNC_END = 269,
		FUNC_INSERT = 260,
		FUNC_DELETE = 261,
		BACKSPACE = 259,
		SPACE = 32,
		ENTER = 257,
		ESCAPE = 256,
		KEY_A = 65,
		KEY_C = 67,
		KEY_V = 86,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
	};

	class OSIRIS_API KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class OSIRIS_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	protected:

		int m_RepeatCount;
	};

	class OSIRIS_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}