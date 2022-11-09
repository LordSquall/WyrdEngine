#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Wyrd
{
    void CustomSerialisation_ScriptComponent(jsonxx::Object& obj, Wyrd::ScriptComponent* data);
    void CustomDeserialisation_ScriptComponent(Wyrd::ScriptComponent* data, jsonxx::Object& obj);
};