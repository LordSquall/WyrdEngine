#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/UID.h"
#include "core/maths/Rect.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Mesh.h"

/* external includes */
#include <glm/glm.hpp>
#include <jsonxx.h>

namespace Wyrd
{
    void SerialiseTexture(std::ofstream& os, std::shared_ptr<Wyrd::Texture> data);
    std::shared_ptr<Wyrd::Texture> DeserialiseTexture(std::ifstream& is);

    void SerialiseMaterial(std::ofstream& os, std::shared_ptr<Wyrd::Material> data);
    std::shared_ptr<Wyrd::Material> DeserialiseMaterial(std::ifstream& is);

    void SerialiseModel(std::ofstream& os, std::shared_ptr<Wyrd::Mesh> data);
    std::shared_ptr<Wyrd::Mesh> DeserialiseModel(std::ifstream& is);

    void SerialiseShader(std::ofstream& os, std::shared_ptr<Wyrd::Shader> data);
    std::shared_ptr<Wyrd::Shader> DeserialiseShader(std::ifstream& is);
}