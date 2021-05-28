#include "C_Scene.h" 
#include "core/scene/Scene.h"
#include "core/ecs/Components.h"


void Scene_BuildComponentList(void* scenePtr)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;

	int metaDataComponentID			= GetID<MetaDataComponent>();
	int transform2DComponenttID		= GetID<ECSTransform2DComponent>();
	int spriteComponentID			= GetID<ECSSpriteComponent>();
	int scriptComponentID			= GetID<ECSScriptComponent>();
	int scriptInternalComponentID	= GetID<ECSScriptInternalComponent>();
	int cameraComponentID			= GetID<ECSCameraComponent>();

	std::cout << "MetaDataComponent Type ID "			<< std::to_string(metaDataComponentID) << std::endl;
	std::cout << "ECSTransform2DComponent Type ID "		<< std::to_string(transform2DComponenttID) << std::endl;
	std::cout << "ECSSpriteComponent Type ID "			<< std::to_string(spriteComponentID) << std::endl;
	std::cout << "ECSScriptComponent Type ID "			<< std::to_string(scriptComponentID) << std::endl;
	std::cout << "ECSScriptInternalComponent Type ID "	<< std::to_string(scriptInternalComponentID) << std::endl;
	std::cout << "ECSCameraComponent Type ID "			<< std::to_string(cameraComponentID) << std::endl;
}

void* Scene_GetComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	void* rawPtr = scene->Get(componentPoolIndex, entity);

	return rawPtr;
}