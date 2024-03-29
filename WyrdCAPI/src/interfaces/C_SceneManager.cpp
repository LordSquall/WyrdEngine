#include "C_SceneManager.h" 
#include "core/scene/Scene.h"
#include "core/ecs/ECS.h"
#include "core/ecs/Components.h"
#include "core/interfaces/ISceneManager.h"


void SceneManager_ChangeScene(void* sceneManagerPtr, const char* sceneName)
{
	Wyrd::ISceneManager* sceneManager = (Wyrd::ISceneManager*)sceneManagerPtr;

	std::string sceneNameStr = sceneName;
	sceneManager->ChangeScene(sceneNameStr);
}