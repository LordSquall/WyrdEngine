#pragma once

/* local includes  */
#include "core/Structures.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>

class ComponentSerialiserFactory
{
public:
	static void Deserialise(const std::string& name, std::ifstream& stream, char* buffer);

	static void Deserialise(std::ifstream& obj, Wyrd::MetaDataComponent* data);
	static void Deserialise(std::ifstream& obj, Wyrd::Transform2DComponent* data);
	static void Deserialise(std::ifstream& obj, Wyrd::SpriteComponent* data);
	static void Deserialise(std::ifstream& obj, Wyrd::ScriptComponent* data);
	static void Deserialise(std::ifstream& obj, Wyrd::CameraComponent* data);
};