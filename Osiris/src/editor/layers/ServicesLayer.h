#pragma once

#include "core/export.h"
#include "core/Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "editor/EditorPlugin.h"
#include "editor/support/IconLibrary.h"
#include "editor/support/Utils.h"

#include "editor/services/ServiceManager.h"

namespace Osiris::Editor
{
	class  ServicesLayer : public Layer
	{
	public:
		ServicesLayer();
		~ServicesLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnRender(Timestep ts, Renderer& renderer) override;
	};
}