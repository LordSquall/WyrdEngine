#include "wyrdpch.h"
#include "LayerStack.h"

namespace Wyrd {

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	bool LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
		return layer->OnAttach();
	}

	bool LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		return overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
		layer->OnDetach();
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);

		overlay->OnDetach();
	}

	Layer* LayerStack::FindLayer(std::string name)
	{
		for (auto layer : m_Layers)
		{
			if (layer->GetName().compare(name) == 0)
				return layer;
		}
		return NULL;
	}

}