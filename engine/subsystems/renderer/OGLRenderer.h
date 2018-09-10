#ifndef _OGL_RENDERER_H_
#define _OGL_RENDERER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"
#include "subsystems\interfaces\IWindow.h"

#include "subsystems\graphics\VertexArray.h"
#include "subsystems\graphics\VertexBuffer.h"
#include "subsystems\graphics\Shader.h"

#include "GL\glew.h"

namespace OrisisEngine
{
	class OGLRenderer : public IRenderer {
		public:
			OGLRenderer();
			OGLRenderer(const OGLRenderer &obj);
			~OGLRenderer();

			/* ISubSystem Functions */
			void RegisterLogger(ILogger* logger);
			ILogger* GetLogger();

			/* IRenderer Functions */			
			bool Initialise(IWindow* window);
			void Shutdown();

			bool CreateTexture(Texture* texture);
			bool UpdateTexture(Texture* texture);
			bool SetTexture(Texture* texture);

			bool RegisterShader(Shader* shader);
			bool UseShader(Shader* shader);

			bool CreateVertexArray(VertexArray* vertexArray);
			bool DeleteVertexArray(VertexArray* vertexArray);

			bool CreateVertexBuffer(VertexBuffer* vertexBuffer);
			bool DeleteVertexBuffer(VertexBuffer* vertexBuffer);
			bool UpdateVertexBuffer(VertexBuffer* vertexBuffer, unsigned int offset, unsigned int size);

			bool DrawVertexBuffer(VertexBuffer* vertexBuffer);
	public:
		ILogger* _logger;
	};
}

#endif