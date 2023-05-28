#pragma once

/* local includes */
#include "core/export.h"
#include "core/UID.h"
#include "core/Structures.h"
#include "core/ecs/ECS.h"
#include "core/pipeline/Vertex2D.h"
#include "core/pipeline/Vertex3D.h"
#include "core/renderer/RendererDrawTypes.h"
#include "properties/BaseProp.h"

/* external includes */
#include <string>
#include <glm/glm.hpp>

namespace Wyrd
{
	class Material;
	class Texture;
	class Mesh;
	class FontType;

	struct WYRD_LIBRARY_API DrawCommand
	{
		uint32_t type;
		glm::mat4 vpMatrix;
		Material* material;
	};

	struct WYRD_LIBRARY_API DrawMeshCommand : public DrawCommand
	{
		RendererDrawType drawType;
		glm::mat4 modelMatrix;
		Color color;
		Mesh* mesh;
		Texture* baseTexture;
		BasePropMapRef* materialProps;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	};
	struct WYRD_LIBRARY_API DrawSpriteCommand : public DrawCommand
	{
		Wyrd::Vector2 position;
		Wyrd::Vector2 rotationOrigin;
		float rotation;
		Wyrd::Vector2 size;
		Wyrd::Vector2 tiling;
		Texture* texture;
		Color color;
	};

	struct WYRD_LIBRARY_API DrawRectCommand : public DrawCommand
	{
		Wyrd::Vector2 position;
		Wyrd::Vector2 rotationOrigin;
		float rotation;
		Wyrd::Vector2 size;
		float thickness;
		Color color;
	};

	struct WYRD_LIBRARY_API DrawTextCommand : public DrawCommand
	{
		Wyrd::Vector2 position;
		float scale;
		std::string content;
		FontType* font;
		Color color;
		float size;
	};

	struct WYRD_LIBRARY_API DrawVertex2DCommand : public DrawCommand
	{
		Wyrd::Vector2 position;
		std::vector<Vertex2D>* vertices;
		Color color;
		RendererDrawType drawType;
	};

	struct WYRD_LIBRARY_API DrawVertex3DCommand : public DrawCommand
	{
		Wyrd::Vector3 position;
		std::vector<Vertex3D>* vertices;
		Color color;
		RendererDrawType drawType;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};

	struct WYRD_LIBRARY_API DrawMeshInputColorPickerCommand : public DrawCommand
	{
		DrawMeshCommand meshCommand;
		Entity entity;
	};

	struct WYRD_LIBRARY_API DrawDebugBoundingBoxCommand : public DrawCommand
	{
		Wyrd::Vector3 position;
		Color color;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};
}