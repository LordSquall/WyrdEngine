#pragma once

#include "core/renderer/commands/RendererCommands.h"

using namespace Wyrd;

namespace Wyrd {

	class Renderer;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	/* represents a single sprite vertex */
	struct SpriteVertex2
	{
		float x, y;		/* 2D position		*/
		float u, v;		/* texture coords	*/
	};

	/* respresents a batch a sprint geometry */
	class SpriteBatch
	{
		/* allow access to internal vertices and index buffers */
		friend struct SpriteBatchEntry;

	public:
		~SpriteBatch() = default;

		bool Initialise(Renderer* renderer);

		void Submit(DrawSpriteCommand& cmd);

		void Flush();

	private:
		Renderer*						_Renderer;
		std::vector<SpriteVertex2>		_vertices;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;

		uint32_t						_SpriteCount;
		//glm::mat4						_VPMatrix;
		Shader*							_Shader;
		Texture*						_Texture;
		Color							_Color;
	};
}