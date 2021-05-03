#pragma once

#include "core/export.h"
#include "Layer.h"

#include <vector>

namespace Wyrd {

	/**
	 * @brief LayerStack Subsystem
	 * Holds the current list of application layers and provides user functions to add, remove and modify current layers
	*/
	class WYRD_LIBRARY_API LayerStack
	{
	public: 
		/**
		 * @brief Constructor
		*/
		LayerStack();
		~LayerStack();

		/**
		 * @brief Add a layer to the stack
		 * @param layer 
		 * @return bool - result of successful attach
		*/
		bool PushLayer(Layer* layer);

		/**
		 * @brief Add a layer to the stack as an Overlay
		 * @param layer
		 * @return bool - result of successful attach
		*/
		bool PushOverlay(Layer* layer);

		/**
		 * @brief Remove a layer from the stack
		 * @param layer 
		*/
		void PopLayer(Layer* layer);

		/**
		 * @brief Remove an Overlay from the stack
		 * @param layer 
		*/
		void PopOverlay(Layer* layer);

		/**
		 * @brief Find a layer within the stack with a specified name
		 * @param name 
		 * @return Layer* - found layer. NULL is layer is not found
		*/
		Layer* FindLayer(std::string name);
		
		/* std::iterator functions*/
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }


	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator  m_LayerInsert;
	};

}