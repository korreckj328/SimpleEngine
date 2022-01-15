#include "RenderTexture.h"
#include "VulkanContext.h"
#include "Tools.h"
#include <vector>

RenderTexture::RenderTexture() {

}

RenderTexture::~RenderTexture() {

}

void RenderTexture::createViewsAndFramebuffer(
					std::vector<VkImage> swapChainImages,
					VkFormat swapChainImageFormat,
					VkExtent2D swapChainImageExtent,
					VkRenderPass renderPass) {
	_swapChainImages = swapChainImages;
	_swapChainImageExtent = swapChainImageExtent;

	createImageViews(swapChainImageFormat);
	createFrameBuffer(swapChainImageExtent, renderPass);
}

void RenderTexture::createImageViews(VkFormat swapChainImageFormat) {
	swapChainImageViews.resize(_swapChainImages.size());

	for(size_t i = 0; i < _swapChainImages.size(); i++) {
		swapChainImageViews[i] = vkTools::createImageView(_swapChainImages[i],
									swapChainImageFormat,
									VK_IMAGE_ASPECT_COLOR_BIT);
	}
}


void RenderTexture::createFrameBuffer(VkExtent2D swapChainImageExtent,
									VkRenderPass renderPass) {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		std::array<VkImageView, 1> attachments = {swapChainImageViews[i]};

		VkFramebufferCreateInfo fbInfo = {};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = renderPass;
		fbInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		fbInfo.pAttachments = attachments.data();
		fbInfo.width = swapChainImageExtent.width;
		fbInfo.height = swapChainImageExtent.height;
		fbInfo.layers = 1;

		if (vkCreateFramebuffer(VulkanContext::getInstance()->getDevice()->logicalDevice,
								&fbInfo,
								NULL,
								&swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffers\n");
		}

	}
}

void RenderTexture::destroy() {
	// destroy image views
	for (auto imageView: swapChainImageViews) {
		vkDestroyImageView(VulkanContext::getInstance()->getDevice()->logicalDevice,
							imageView,
							nullptr);
	}

	// destroy framebuffers
	for (auto framebuffer: swapChainFramebuffers) {
		vkDestroyFramebuffer(VulkanContext::getInstance()->getDevice()->logicalDevice,
							framebuffer,
							nullptr);
	}
}
