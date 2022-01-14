#include <vulkan/vulkan.h>
#include <array>

class Renderpass {
	Renderpass();
	~Renderpass();

	VkRenderPass renderPass;

	void createRenderPass(VkFormat swapChainImageFormat);

	void destroy();
};

