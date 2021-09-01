#pragma once

#include "core/renderer/commands/RendererCommands.h"
#include "core/pipeline/Vertex2D.h"

using namespace Wyrd;

namespace Wyrd {

	class Renderer;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	/* respresents a batch a sprint geometry */
	class Vertex2DBatch
	{
	public:
		~Vertex2DBatch() = default;

		bool Initialise(Renderer* renderer);

		void Submit(DrawRectCommand& cmd);
		void Submit(DrawVertex2DCommand& cmd);

		void Flush();

	private:
		Renderer*						_Renderer;
		std::vector<Vertex2D>			_vertices;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;

		glm::mat4						_VPMatrix;
		Shader*							_Shader;
		Color							_Color;
	};
}