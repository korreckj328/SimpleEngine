#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <vector>
#include <vulkan/vulkan.h>
#include <array>

class RenderTexture {
public:
	RenderTexture();
	~RenderTexture();
	std::vector<VkImage> _swapChainImages;
	VkExtent2D _swapChainImageExtent;

	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	void createViewsAndFramebuffer(std::vector<VkImage> swapChainImages,
									VkFormat swapChainImageFormat,
									VkExtent2D swapChainImageExtent,
									VkRenderPass renderPass);

	void createImageViews(VkFormat swapChainImageFormat);
	void createFrameBuffer(VkExtent2D swapChainImageExtent,
							VkRenderPass renderPass);

	void destroy();
};


#endif
