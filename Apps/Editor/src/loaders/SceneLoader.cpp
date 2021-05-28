#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/scene/Scene.h>
#include <core/scene/Layer2D.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/Transform3DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/components/CameraComponent.h>
#include <core/renderer/Texture.h>
#include <core/ecs/Components.h>

/* local includes */
#include "SceneLoader.h"
#include "services/ServiceManager.h"
#include "services/ResourceService.h"
#include "extensions/systems/EditorSystem.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

using namespace glm;

namespace Wyrd::Editor
{
	static std::shared_ptr<ResourceService> _resourceService;
	static std::shared_ptr<CoreSystemsService> _coreSystemService;
	static PropertyList_t _resolveProperties;

	SceneLoader::Result SceneLoader::Load(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;
		jsonxx::Object o;

		_resourceService = ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources);
		_coreSystemService = ServiceManager::Get<CoreSystemsService>(ServiceManager::Service::CoreSystems);

		std::ifstream f(path);

		if (f.is_open() == true) {
			std::ostringstream ss;
			ss << f.rdbuf();

			if (o.parse(ss.str()) == true)
			{
				scene.FromJson(o);
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

		//scene.Initialise();

		///* register the additional editor component */
		//scene.RegisterComponent<ECSEditorComponent>("Editor");

		///* check if the matching binary file is present */
		//if (Utils::FileExists(path + ".bin"))
		//{
		//	std::ifstream binfile(path + ".bin", std::ios::in | std::ios::binary);

		//	uint32_t entityCount = 0;
		//	binfile.read((char*)&entityCount, sizeof(uint32_t));

		//	for (uint32_t i = 0; i < entityCount; i++)
		//	{
		//		unsigned long long id = 0;
		//		unsigned long long mask = 0;

		//		binfile.read((char*)&id, sizeof(unsigned long long));
		//		binfile.read((char*)&mask, sizeof(unsigned long long));

		//		scene.entities.push_back({ id, ComponentMask(mask) });
		//	}

		//	uint32_t componentPoolCnt = 0;
		//	binfile.read((char*)&componentPoolCnt, sizeof(uint32_t));

		//	for (uint32_t i = 0; i < componentPoolCnt; i++)
		//	{
		//		uint32_t componentSize = 0;
		//		uint32_t componentElementCount = 0;

		//		binfile.read((char*)&componentSize, sizeof(uint32_t));
		//		binfile.read((char*)&scene.componentPools[i]->count, sizeof(uint32_t));
		//						
		//		binfile.read(scene.componentPools[i]->data, componentSize * MAX_ENTITIES);
		//	}
		//	binfile.close();
		//}
		//else
		//{
		//}

		// TEMP TEST ECS Data set
		//for (auto& l : scene.GetLayers())
		//{
		//	for (auto& go : l->GetGameObjects())
		//	{
		//		/* create the entity */
		//		Entity entity = scene.CreateEntity();

		//		/* add the meta data */
		//		MetaDataComponent* metadataComp = scene.AssignComponent<MetaDataComponent>(entity);
		//		strcpy(metadataComp->name, go->name.c_str());

		//		/* add the editor component */
		//		ECSEditorComponent* editorComp = scene.AssignComponent<ECSEditorComponent>(entity);
		//		editorComp->inputArea = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };

		//		{
		//			Transform2DComponent* c = dynamic_cast<Transform2DComponent*>(go->transform.get());

		//			ECSTransform2DComponent* ecsC = scene.AssignComponent<ECSTransform2DComponent>(entity);
		//			ecsC->position = c->position;
		//			ecsC->rotation = c->rotation;
		//		}

		//		for (auto& comp : go->components)
		//		{
		//			if (comp->GetType() == SceneComponentType::Transform2D)
		//			{
		//				Transform2DComponent* c = dynamic_cast<Transform2DComponent*>(comp.get());

		//				ECSTransform2DComponent* ecsC = scene.AssignComponent<ECSTransform2DComponent>(entity);
		//				ecsC->position = c->position;
		//				ecsC->rotation = c->rotation;
		//			}

		//			if (comp->GetType() == SceneComponentType::SpriteRenderer)
		//			{
		//				SpriteComponent* c = dynamic_cast<SpriteComponent*>(comp.get());

		//				ECSSpriteComponent* ecsC = scene.AssignComponent<ECSSpriteComponent>(entity);
		//				ecsC->position = c->position;
		//				ecsC->size = c->size;
		//				ecsC->texture = c->GetTexture()->GetUID();
		//				ecsC->color = c->color;
		//			}
		//			
		//			if (comp->GetType() == SceneComponentType::ScriptComponent)
		//			{
		//				ScriptComponent* c = dynamic_cast<ScriptComponent*>(comp.get());

		//				ECSScriptComponent* ecsC = scene.AssignComponent<ECSScriptComponent>(entity);
		//				ecsC->script = c->GetUID();

		//				ECSScriptInternalComponent* ecsCI = scene.AssignComponent<ECSScriptInternalComponent>(entity);
		//				ecsCI->instanceID = 0;
		//			}

		//			if (comp->GetType() == SceneComponentType::CameraComponent)
		//			{
		//				CameraComponent* c = dynamic_cast<CameraComponent*>(comp.get());

		//				ECSCameraComponent* ecsC = scene.AssignComponent<ECSCameraComponent>(entity);
		//				ecsC->size = c->GetSize();
		//			}
		//		}
		//	}
		//}

		return result;
	}

	SceneLoader::Result SceneLoader::Save(std::string path, Scene& scene)
	{
		SceneLoader::Result result = Success;

		/* construct the scene into a json file */
		jsonxx::Object o = scene.ToJson();
		std::ofstream out(path);
		out << o.json();
		out.close();

		//std::ofstream binFile(path +".bin", std::ios::out | std::ios::binary);

		///* write the entity list and masks */
		//uint32_t entityCount = (uint32_t)scene.entities.size();
		//binFile.write((char*)&entityCount, sizeof(uint32_t));
		//for (auto& e : scene.entities)
		//{
		//	unsigned long long mask = e.mask.to_ullong();

		//	binFile.write((char*)&e.id, sizeof(unsigned long long));
		//	binFile.write((char*)&mask, sizeof(unsigned long long));
		//}

		//uint32_t componentPoolCount = (uint32_t)scene.componentPools.size();
		//binFile.write((char*)&componentPoolCount, sizeof(uint32_t));

		//for (auto& cp : scene.componentPools)
		//{
		//	binFile.write((char*)&cp->elementSize, sizeof(uint32_t));
		//	binFile.write((char*)&cp->count, sizeof(uint32_t));
		//	binFile.write(cp->data, cp->elementSize * MAX_ENTITIES);
		//}
		//binFile.close();

		return result;
	}
}
