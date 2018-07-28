#ifndef _DX11_RENDERER_H_
#define _DX11_RENDERER_H_

#include "export.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

namespace OrisisEngine
{
	class DX11Renderer : public IRenderer {
		public:
			DX11Renderer();
			DX11Renderer(const DX11Renderer &obj);
			~DX11Renderer();

			bool Initialise();
			void Shutdown();
	};
}

#endif