#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_

#include <iostream>
#include <string.h>

#include "ILogger.h"

#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\IWindow.h"

#include "subsystems\common\structures.h"
#include "subsystems\graphics\Texture.h"
#include "subsystems\graphics\Shader.h"
#include "subsystems\graphics\VertexArray.h"
#include "subsystems\graphics\VertexBuffer.h"

using namespace std;

namespace OrisisEngine
{
	class IRenderer : public ISubSystem {
	public:
		virtual bool Initialise(IWindow* window) = 0;
		virtual void Shutdown() = 0;

		/* Texture Functions */
		virtual bool CreateTexture(Texture* texture) = 0;
		virtual bool UpdateTexture(Texture* texture) = 0;
		virtual bool SetTexture(Texture* texture) = 0;

		/* Shader Functions */
		virtual bool RegisterShader(Shader* shader) = 0;
		virtual bool UseShader(Shader* shader) = 0;

		/* Vertex Array Functions */
		virtual bool CreateVertexArray(VertexArray* vertexArray) = 0;
		virtual bool DeleteVertexArray(VertexArray* vertexArray) = 0;

		/* Vertex Buffer Functions */
		virtual bool CreateVertexBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual bool DeleteVertexBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual bool UpdateVertexBuffer(VertexBuffer* vertexBuffer, unsigned int offset, unsigned int size) = 0;

		/* Drawing Functions */
		virtual bool DrawVertexBuffer(VertexBuffer* vertexBuffer) = 0;
	};
}

#endif