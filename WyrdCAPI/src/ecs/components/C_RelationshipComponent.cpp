/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_RelationshipComponent.h" 

void RelationshipComponent_SetFirst(void* scenePtr, Wyrd::Entity entity, Wyrd::Entity first)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->first = first;
}

void RelationshipComponent_SetPrevious(void* scenePtr, Wyrd::Entity entity, Wyrd::Entity previous)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->previous = previous;
}

void RelationshipComponent_SetNext(void* scenePtr, Wyrd::Entity entity, Wyrd::Entity next)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->next = next;
}

void RelationshipComponent_SetParent(void* scenePtr, Wyrd::Entity entity, Wyrd::Entity parent)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->parent = parent;
}

void RelationshipComponent_SetChildrencnt(void* scenePtr, Wyrd::Entity entity, uint32_t childrenCnt)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->childrenCnt = childrenCnt;
}

void RelationshipComponent_SetDepth(void* scenePtr, Wyrd::Entity entity, int32_t depth)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->depth = depth;
}

void RelationshipComponent_SetRemove(void* scenePtr, Wyrd::Entity entity, bool remove)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::RelationshipComponent* component = scene->Get<Wyrd::RelationshipComponent>(entity);

   component->remove = remove;
}


