#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/FontType.h>

/* local includes */
#include "FontTypeRes.h"
#include "loaders/TextureLoader.h"
#include "services/ServiceManager.h"

/* external includes */
#include <core/Application.h>

namespace Wyrd::Editor
{
	FontTypeRes::FontTypeRes(const std::string& filepath) : Resource(Utils::GetFilename(filepath), filepath)
	{

	}
	
	bool FontTypeRes::Load()
	{
		/* textures should always be loaded on the main thread */
		if (Application::Get().GetMainThreadID() != std::this_thread::get_id())
		{
			/* swap an event to trigger the load on the main thread */
			ServiceManager::Get<EventService>(ServiceManager::Events)->Publish(Events::EventType::LoadAsset, std::make_unique<Events::LoadAssetArgs>(this));
		}
		else
		{
			/* load the data from file */
			_fontType = FontType::CreateFromTTFFile("fonts/droid-sans-mono/DroidSansMono.ttf");

			/* register the font type with the core resource manager */
			Application::Get().GetResources().FontTypes[_resourceID] = _fontType;
		}

		_loaded = true;

		return _loaded;
	}
}
