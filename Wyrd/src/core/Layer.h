#pragma once

#include "core/export.h"
#include "events/Event.h"

#include "core/renderer/Renderer.h"
#include "core/Timestep.h"

namespace Wyrd {

	/**
	 * @brief Applicaiton Layer
	 * A Layer is a object that lives within the application and is subject to the application lifecycle function callbacks.
	*/
	class WYRD_LIBRARY_API Layer
	{
	public:
		/**
		 * @brief Constructor
		 * @param name 
		*/
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		/**
		 * @brief Callback function called when the layer is attached to the application
		 * @return bool - attachment status
		*/
		virtual bool OnAttach() { return true; }

		/**
		 * @brief Callback function called when the layer is detached from the application
		 * @return bool - detachement status
		*/
		virtual void OnDetach() {}

		/**
		 * @brief Callback function called as part of the application main loop.
		 * @param ts - delta time since last OnUpdate call
		*/
		virtual void OnUpdate(Timestep ts) {}

		/**
		 * @brief Callback function called as part of the application main loop.
		 * @param ts - delta time since last OnUpdate call
		 * @param renderer - renderer subsystem
		*/
		virtual void OnRender(Timestep ts, Renderer& renderer) {}

		/**
		 * @brief Callback function called when a new application event is recieved
		 * @param event - event recieved
		*/
		virtual void OnEvent(Event& event) {}

		/**
		 * @brief Get the layer name
		 * @return string - layer name
		*/
		inline const std::string& GetName() const { return _Name; }

		/**
		 * @brief Check if the layer is currently enabled
		 * @return bool - is layer enabled
		*/
		inline bool IsEnabled() { return _Enabled; }

		/**
		 * @brief Set the enabled status of the layer
		 * @param flag - new enabled status
		*/
		inline void SetEnabled(const bool flag) { _Enabled = flag; }

	protected:
		bool _Enabled;
		std::string _Name;
	};
}