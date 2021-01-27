#pragma once

/* local includes */
#include "../C_Export.h"

/* core osiris includes */
#include <core/behaviour/Behaviour.h>

#ifdef __cplusplus
extern "C" {
#endif

extern Osiris::Behaviour* _behaviour;

LIBRARY_API void Manager_SetBehaviour(void* behaviour);

#ifdef __cplusplus
}
#endif