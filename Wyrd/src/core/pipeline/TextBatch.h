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
	struct TextVertex2D
	{
		float x, y;		/* 2D position		*/
		float u, v;		/* UV               */
	};

	/* respresents a batch a sprint geometry */
	class TextBatch
	{
	public:
		~TextBatch() = default;

		bool Initialise(Renderer* renderer);

		void Submit(DrawTextCommand& cmd);

		void Flush();

	private:
		Renderer*						_Renderer;
		std::vector<TextVertex2D>		_vertices;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;

		glm::mat4						_VPMatrix;
		Shader*							_Shader;
		Color							_Color;
	};
}