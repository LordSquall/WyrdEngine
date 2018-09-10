#include "VulkanRenderer.h"

#include <vector>

using namespace OrisisEngine;

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::VulkanRenderer(const VulkanRenderer &obj)
{
}

VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::RegisterLogger(ILogger* logger)
{
	_logger = logger;
}

bool VulkanRenderer::Initialise(IWindow* window)
{
	///* Validation Layer list, including the standard LunarG layer */
	//const std::vector<const char*> validationLayers = {
	//	"VK_LAYER_LUNARG_standard_validation"
	//};

	///* Define a Vulkan Application */
	//VkApplicationInfo appInfo = {};
	//appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//appInfo.pApplicationName = "Hello Vulkan";
	//appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	//appInfo.pEngineName = "No Engine";
	//appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	//appInfo.apiVersion = VK_API_VERSION_1_0;

	///* Define a Vulkan Instance */
	//VkInstanceCreateInfo createInfo = {};
	//createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	//createInfo.pApplicationInfo = &appInfo;

	///* Create a Vulkan Instance*/
	//VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	//if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create instance!");
	//}


	//auto extensions = getRequiredExtensions();
	//createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	//createInfo.ppEnabledExtensionNames = extensions.data();

	//if (enableValidationLayers) {
	//	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	//	createInfo.ppEnabledLayerNames = validationLayers.data();
	//}
	//else {
	//	createInfo.enabledLayerCount = 0;
	//}

	//if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create instance!");
	//}

	return true;
}

void VulkanRenderer::Shutdown()
{
	vkDestroyInstance(instance, nullptr);
}