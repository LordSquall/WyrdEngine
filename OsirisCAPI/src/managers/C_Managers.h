#pragma once

/* local includes */
#include "../C_Export.h"

/* core osiris includes */
#include <core/behaviour/Behaviour.h>
#include <core/physics/Physics.h>
#include <core/Resources.h>

#ifdef __cplusplus
extern "C" {
#endif

extern Osiris::Behaviour* _behaviour;
extern Osiris::Physics* _physics;
extern Osiris::Resources* _resources;

LIBRARY_API void Manager_SetBehaviour(void* behaviour);

LIBRARY_API void Manager_SetPhysics(void* physics);

LIBRARY_API void Manager_SetResources(void* resources);

#ifdef __cplusplus
}
#endif