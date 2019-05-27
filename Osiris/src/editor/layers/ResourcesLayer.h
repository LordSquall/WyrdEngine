#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "events/Event.h"


namespace Osiris::Editor
{
	class ResourcesLayer : public Layer
	{
	public:
		ResourcesLayer();
		~ResourcesLayer();

		
		void OnAttach() override;
		void OnDetach() override;
		void OnRender(std::shared_ptr<Renderer> renderer) override;
		void OnEvent(Event& event) override;
		
	};
}