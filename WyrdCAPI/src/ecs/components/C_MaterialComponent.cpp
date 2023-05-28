/* core wyrd includes */
#include <properties/BaseProp.h>
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_MaterialComponent.h" 

void MaterialComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->enabled = enabled;
}

void MaterialComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->color = *color;
}

void MaterialComponent_SetMaterial(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* material)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->material = *material;
}

void MaterialComponent_SetProperty_Int(void* scenePtr, Wyrd::Entity entity, char* name, int value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<int>(value);
}

void MaterialComponent_SetProperty_UInt32(void* scenePtr, Wyrd::Entity entity, char* name, uint32_t value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<uint32_t>(value);
}

void MaterialComponent_SetProperty_Int64(void* scenePtr, Wyrd::Entity entity, char* name, int64_t value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<int64_t>(value);
}

void MaterialComponent_SetProperty_UInt64(void* scenePtr, Wyrd::Entity entity, char* name, uint64_t value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<uint64_t>(value);
}

void MaterialComponent_SetProperty_Float(void* scenePtr, Wyrd::Entity entity, char* name, float value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<float>(value);
}

void MaterialComponent_SetProperty_Double(void* scenePtr, Wyrd::Entity entity, char* name, double value)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<double>(value);
}

void MaterialComponent_SetProperty_Color(void* scenePtr, Wyrd::Entity entity, char* name, Wyrd::Color* colorPtr)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);
	(*component->properties)[name]->Set<Wyrd::Color>(*colorPtr);
}

void MaterialComponent_SetProperty_Texture(void* scenePtr, Wyrd::Entity entity, char* name, void* texturePtr)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::Texture* texture = (Wyrd::Texture*)texturePtr;
	Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

	(*component->properties)[name]->Set<Wyrd::Texture*>(texture);
}

