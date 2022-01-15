#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vulkan/vulkan.h>
#include <array>

class Renderpass {
public:
	Renderpass();
	~Renderpass();

	VkRenderPass renderPass;

	void createRenderPass(VkFormat swapChainImageFormat);

	void destroy();
};

#endif
