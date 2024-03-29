#pragma once

// This layer should be able enabled if the rendererdoc symbol is 
#ifdef WYRD_RENDERDOC_ENABLED

#include "core/export.h"
#include "core/Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

#include "views/EditorViewBase.h"
#include "support/IconLibrary.h"
#include "support/Utils.h"

#include "services/ServiceManager.h"

#include "renderdoc_app.h"

namespace Wyrd::Editor
{
	class  RenderDocLayer : public Layer
	{
	public:
		RenderDocLayer();
		~RenderDocLayer();

		bool OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnRender(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;

	private:
		void UpdateSettings();

		void OnSettingsUpdated(Events::EventArgs& args);

		bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);
		

		RENDERDOC_API_1_1_2* _RDOCAPI = NULL;
		bool _CaptureRunning = false;
		unsigned int _FrameCounter = 0;
		unsigned int _FrameCaptureCount = 0;

		std::shared_ptr<SettingsService> _SettingsService;
	};
}

#endif