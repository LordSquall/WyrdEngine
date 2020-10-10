#pragma once

#include "export.h"

namespace Osiris
{
	/**
	 * @brief Input Subsystem
	 * This class provides a common interface to the Input System
	*/
	class OSR_LIBRARY_API Input
	{
	public:

		/**
		 * @brief Check if a specific key is currently pressed
		 * @param keycode 
		 * @return bool - Pressed state
		*/
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		/**
		 * @brief Check if a specific mouse button is currently pressed
		 * @param button 
		 * @return bool - Pressed state
		*/
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }

		/**
		 * @brief Get the current mouse position's x coordinate
		 * @return float - current mouse x coord
		*/
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }

		/**
		 * @brief Get the current mouse position's y coordinate
		 * @return float - current mouse y coord
		*/
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		/**
		 * @brief Get the current mouse position
		 * @return float, float - current mouse coord
		*/
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual bool GetMouseXImpl() = 0;
		virtual bool GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		static Input* s_Instance;
	};
}