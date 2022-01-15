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

	void beginRenderPass(std::array<VkClearValue, 1> clearValues,
						VkCommandBuffer commandBuffer, 
						VkFramebuffer swapChainFrameBuffer,
						VkExtent2D swapChainImageExtent);

	void endRenderPass(VkCommandBuffer commandbuffer);

	void destroy();
};

#endif
