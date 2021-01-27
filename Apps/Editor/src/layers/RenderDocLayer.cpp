#pragma once

// This layer should be able enabled if the rendererdoc symbol is defined
#ifdef OSR_RENDERDOC_ENABLED

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/KeyCodes.h>
#include <events/Event.h>
#include <events/KeyEvent.h>
#include <events/MouseEvent.h>
#include <events/ApplicationEvent.h>

/* local includes */
#include "RenderDocLayer.h"

namespace Osiris::Editor
{
	RenderDocLayer::RenderDocLayer()
		: Layer("RenderDocLayer")
	{
		if (HMODULE mod = LoadLibraryA("renderdoc.dll"))
		{
			pRENDERDOC_GetAPI RENDERDOC_GetAPI =
				(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
			int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_4_1, (void**)&_RDOCAPI);

			if (ret == 1)
			{
				OSR_INFO("RenderDoc GetApi Successfully connected.");
			}
			else
			{
				OSR_ERROR("Attempted to load RenderDoc DLL GetAPI function. However unable to resolve.");
			}
		}
	}

	RenderDocLayer::~RenderDocLayer()
	{

	}

	bool RenderDocLayer::OnAttach()
	{
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Service::Settings);

		UpdateSettings();

		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SettingsUpdated, EVENT_FUNC(RenderDocLayer::OnSettingsUpdated));

		return true;
	}

	void RenderDocLayer::OnDetach()
	{

	}

	void RenderDocLayer::OnRender(Timestep ts, Renderer& renderer)
	{
	}

	void RenderDocLayer::OnUpdate(Timestep ts)
	{
		if (_CaptureRunning == true)
		{
			if (_FrameCounter == _FrameCaptureCount)
			{
				if (_RDOCAPI != NULL)
				{
					int result = _RDOCAPI->EndFrameCapture(NULL, NULL);
					if (result == 0)
					{
						OSR_ERROR("Failed to capture doc in Render Doc");
					}
					else
					{
						char* filenameBuffer = (char*)malloc(sizeof(char) * Utils::ToUInt(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__MAXFILEPATH, "256")));
						uint32_t pathLength = 0;
						uint64_t timestamp = 0;

						_RDOCAPI->GetCapture(0, &filenameBuffer[0], &pathLength, &timestamp);

						OSR_INFO("Renderdoc Capture Ended. Capture Successful: {0}", filenameBuffer);

						if (Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__AUTOOPEN, "1")))
						{
							Utils::OpenFileWithSystem(std::string(filenameBuffer));
						}

						free(filenameBuffer);

					}
				}
				_CaptureRunning = false;
				_FrameCounter = 0;
			}
			else
			{
				_FrameCounter++;
			}
		}
	}

	void RenderDocLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(RenderDocLayer::OnKeyPressedEvent));
	}

	void RenderDocLayer::UpdateSettings()
	{
		if (_RDOCAPI != NULL)
		{
			if (Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__DISPLAYOVERLAY, "0")) == false)
			{
				_RDOCAPI->MaskOverlayBits(0, 0);
			}
			else
			{
				_RDOCAPI->MaskOverlayBits(~0U, ~0U);
			}
		}
	}


	void RenderDocLayer::OnSettingsUpdated(Events::EventArgs& args)
	{
		UpdateSettings();
	}

	bool RenderDocLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == OSR_KEY_F4)
		{
			if (_RDOCAPI != NULL)
			{
				if (Utils::ToBool(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__ENABLED, "0")))
				{
					_RDOCAPI->SetCaptureFilePathTemplate(_SettingsService->GetSetting(CONFIG_RENDERDOC, CONFIG_RENDERDOC__CAPTUREDIR, "my_captures/example").c_str());
					_RDOCAPI->StartFrameCapture(NULL, NULL);

					int ret = _RDOCAPI->IsFrameCapturing();

					_FrameCounter = 0;
					_CaptureRunning = true;

					OSR_INFO("Renderdoc Capture Started");
					return true;
				}
			}
		}

		return false;
	}
}

#endif