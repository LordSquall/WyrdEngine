#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Wyrd
{
    void CustomSerialisation_MaterialComponent(jsonxx::Object& obj, Wyrd::MaterialComponent* data);
    void CustomDeserialisation_MaterialComponent(Wyrd::MaterialComponent* data, jsonxx::Object& obj);

    void CustomSerialisation_MaterialComponent(std::ofstream& stream, Wyrd::MaterialComponent* data);
    void CustomDeserialisation_MaterialComponent(std::ifstream& stream, Wyrd::MaterialComponent* data);

    void CustomSerialisation_ScriptComponent(jsonxx::Object& obj, Wyrd::ScriptComponent* data);
    void CustomDeserialisation_ScriptComponent(Wyrd::ScriptComponent* data, jsonxx::Object& obj);

    void CustomSerialisation_ScriptComponent(std::ofstream& stream, Wyrd::ScriptComponent* data);
    void CustomDeserialisation_ScriptComponent(std::ifstream& stream, Wyrd::ScriptComponent* data);
};