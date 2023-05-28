#include "C_Scene.h" 
#include "core/scene/Scene.h"
#include "core/ecs/ECS.h"
#include "core/ecs/Components.h"

typedef void*(*componentCtorFunc)(void*, Wyrd::Entity);

std::map<int, componentCtorFunc> _componentCtorFuncs;


void* Scene_AddComponent_MetaData(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::MetaDataComponent>(entity);
}

void* Scene_AddComponent_Transform2D(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::Transform2DComponent>(entity);
}

void* Scene_AddComponent_Transform3D(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::Transform3DComponent>(entity);
}

void* Scene_AddComponent_Relationship(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::RelationshipComponent>(entity);
}

void* Scene_AddComponent_MeshRenderer(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::MeshRendererComponent>(entity);
}

void* Scene_AddComponent_Material(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::MaterialComponent>(entity);
}

void* Scene_AddComponent_Sprite(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::SpriteComponent>(entity);
}

void* Scene_AddComponent_Script(void* scenePtr, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->AssignComponent<Wyrd::ScriptComponent>(entity);
}

void Scene_BuildComponentList(void* scenePtr)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;

	std::cout << "FOOOBAR!" << std::endl;
	int metaDataComponentID			= Wyrd::GetID<Wyrd::MetaDataComponent>();
	int transform2DComponenttID		= Wyrd::GetID<Wyrd::Transform2DComponent>();
	int transform3DComponenttID		= Wyrd::GetID<Wyrd::Transform3DComponent>();
	int relationshipComponentID		= Wyrd::GetID<Wyrd::RelationshipComponent>();
	int meshRendererComponentID		= Wyrd::GetID<Wyrd::MeshRendererComponent>();
	int materialComponentID			= Wyrd::GetID<Wyrd::MaterialComponent>();
	int spriteComponentID			= Wyrd::GetID<Wyrd::SpriteComponent>();
	int scriptComponentID			= Wyrd::GetID<Wyrd::ScriptComponent>();
	//int cameraComponentID			= Wyrd::GetID<Wyrd::CameraComponent>();

	_componentCtorFuncs.emplace(metaDataComponentID, &Scene_AddComponent_MetaData);
	_componentCtorFuncs.emplace(transform2DComponenttID, &Scene_AddComponent_Transform2D);
	_componentCtorFuncs.emplace(transform3DComponenttID, &Scene_AddComponent_Transform3D);
	_componentCtorFuncs.emplace(relationshipComponentID, &Scene_AddComponent_Relationship);
	_componentCtorFuncs.emplace(meshRendererComponentID, &Scene_AddComponent_MeshRenderer);
	_componentCtorFuncs.emplace(materialComponentID, &Scene_AddComponent_Material);
	_componentCtorFuncs.emplace(spriteComponentID, &Scene_AddComponent_Sprite);
	_componentCtorFuncs.emplace(scriptComponentID, &Scene_AddComponent_Script);

	std::cout << "MetaDataComponent Type ID "			<< std::to_string(metaDataComponentID) << std::endl;
	std::cout << "ECSTransform2DComponent Type ID "		<< std::to_string(transform2DComponenttID) << std::endl;
	std::cout << "ECSTransform3DComponent Type ID "		<< std::to_string(transform3DComponenttID) << std::endl;
	std::cout << "ECSRelationshipComponent Type ID "	<< std::to_string(relationshipComponentID) << std::endl;
	std::cout << "ECSMeshRendererComponent Type ID "	<< std::to_string(meshRendererComponentID) << std::endl;
	std::cout << "ECSMaterialComponent Type ID "		<< std::to_string(materialComponentID) << std::endl;
	std::cout << "ECSSpriteComponent Type ID "			<< std::to_string(spriteComponentID) << std::endl;
	std::cout << "ECSScriptComponent Type ID "			<< std::to_string(scriptComponentID) << std::endl;
	//std::cout << "ECSCameraComponent Type ID "			<< std::to_string(cameraComponentID) << std::endl;
}

void* Scene_GetComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	void* rawPtr = scene->Get(componentPoolIndex, entity);

	return rawPtr;
}


void* Scene_AddComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	void* rawPtr = _componentCtorFuncs[componentPoolIndex](scene, entity);
	return rawPtr;
}


Wyrd::Entity Scene_CreateEntity(void* scenePtr, const char* name)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	return scene->CreateEntity(name);
}