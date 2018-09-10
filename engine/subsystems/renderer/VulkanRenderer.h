#ifndef _VULKAN_RENDERER_H_
#define _VULKAN_RENDERER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"
#include "subsystems\interfaces\IWindow.h"

#include <vulkan\vulkan.h>

namespace OrisisEngine
{
	class VulkanRenderer : public IRenderer {
		public:
			VulkanRenderer();
			VulkanRenderer(const VulkanRenderer &obj);
			~VulkanRenderer();

			/* ISubSystem Functions */
			void RegisterLogger(ILogger* logger);

			/* IRenderer Functions */			
			bool Initialise(IWindow* window);
			void Shutdown();

	public:
		ILogger* _logger;

	private:
		VkInstance instance;
	};
}

#endif