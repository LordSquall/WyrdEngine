/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/ecs/Components.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "datamodels/EditorComponents.h"
#include "events/EditorEvents.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static std::shared_ptr<CoreSystemsService> _coreSystemService;
	static PropertyList_t _resolveProperties;

	SceneLoader::Result SceneLoader::Load(const std::filesystem::path& path, Scene& scene, bool includeEditorComponents)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		_resourceService = ServiceManager::Get<ResourceService>();
		_coreSystemService = ServiceManager::Get<CoreSystemsService>();

		/* perform the initial load of the JSON file */
		std::ifstream f(path);
		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				//scene.FromJson(o);
			}
			else
			{
				result = FileMalformed;
			}
		}
		else
		{
			result = FileNotFound;
		}

		/* add additional editor components */
		if (includeEditorComponents)
		{
			for (auto& entity : scene.entities)
			{
				if (entity.mask != 0)
					scene.AssignComponent<EditorComponent>(entity.id);
			}
		}

		/* set the main camera */
		if (scene.GetPrimaryCameraEntity() != ENTITY_INVALID)
		{
			CameraComponent* cameraComponent = scene.Get<CameraComponent>(scene.GetPrimaryCameraEntity());
			if (cameraComponent != nullptr)
			{
				ServiceManager::Get<EventService>()->Publish(Editor::Events::EventType::SetSceneCamera, std::make_unique<Events::SetSceneCameraArgs>(scene.GetPrimaryCameraEntity(), cameraComponent));
			}
			else
			{
				WYRD_ERROR("Selected Camera Entity missing Camera component.");
			}
		}
		else
		{
			WYRD_ERROR("Unable to find Selected Camera Entity.");
		}

		return result;
	}

	SceneLoader::Result SceneLoader::Save(const std::filesystem::path& path, Scene& scene)
	{
		SceneLoader::Result result = Success;

		/* construct the scene into a json file */
		jsonxx::Object o;
		//scene.ToJson(o);

		std::ofstream out(path);
		out << o.json();
		out.close();

		return result;
	}
}
