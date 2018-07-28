#ifndef _OGL_RENDERER_H_
#define _OGL_RENDERER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"


namespace OrisisEngine
{
	class OGLRenderer : public IRenderer {
		public:
			OGLRenderer();
			OGLRenderer(const OGLRenderer &obj);
			~OGLRenderer();

			/* ISubSystem Functions */
			void RegisterLogger(ILogger* logger);

			/* IRenderer Functions */			
			bool Initialise();
			void Shutdown();

	public:
		ILogger* _logger;
	};
}

#endif