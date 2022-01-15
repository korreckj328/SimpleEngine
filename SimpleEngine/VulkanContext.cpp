//
//  VulkanContext.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "VulkanContext.h"
#include <array>
#include <limits>

VulkanContext *VulkanContext::instance = NULL;

VulkanContext *VulkanContext::getInstance() {
    if(!instance) {
        instance = new VulkanContext();
    }
    return instance;
}

VulkanContext::~VulkanContext() {
    std::cout << "Vulkan Context Destroyed" << std::endl;
}

void VulkanContext::initVulkan(GLFWwindow *window) {
    //validation and extension layers
    valLayersAndExt = new AppValidationLayersAndExtensions();
    
    if(isValidationLayersEnabled) {
        std::cout << "Validation Layers Enabled" << std::endl;
    } else {
        std::cout << "Validation Layers Disabled" << std::endl;
    }
    
    if(isValidationLayersEnabled && !valLayersAndExt->checkValidationLayerSupport()) {
        throw std::runtime_error("Validation Layers Not Available! \n");
    }
    
    
    // create app and vulkan instance
    vInstance = new VulkanInstance();
    vInstance->createAppAndVkInstance(isValidationLayersEnabled, valLayersAndExt);
    
    // debug callback
    valLayersAndExt->setupDebugCallback(isValidationLayersEnabled, vInstance->vkInstance);
    
    // create the surface we will do our rendering on
    std::cout << "Creating Window Surface ... ";
    if (glfwCreateWindowSurface(vInstance->vkInstance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("window surface creation failed\n");
    }
    std::cout << "Done!" << std::endl;
    
    // create the logical device
    device = new Device();
    device->pickPhysicalDevice(vInstance, surface);
    device->createLogicalDevice(surface, isValidationLayersEnabled, valLayersAndExt);
    swapChain = new SwapChain();
    swapChain->create(surface);
	renderPass = new Renderpass();
	renderPass->createRenderPass(swapChain->swapChainImageFormat);
	renderTexture = new RenderTexture();
	renderTexture->createViewsAndFramebuffer(swapChain->swapChainImages,
											swapChain->swapChainImageFormat,
											swapChain->swapChainImageExtent,
											renderPass->renderPass);

	drawCommandBuffer = new DrawCommandBuffer();
	drawCommandBuffer->createCommandPoolAndBuffer(swapChain->swapChainImages.size());
}

Device *VulkanContext::getDevice() {
    return device;
}


void VulkanContext::drawBegin() {
	vkAcquireNextImageKHR(VulkanContext::getInstance()->getDevice()->logicalDevice,
					swapChain->swapChain,
					std::numeric_limits<uint64_t>::max(),
					NULL,
					VK_NULL_HANDLE,
					&imageIndex);
	currentCommandbuffer = drawCommandBuffer->commandBuffers[imageIndex];
	
	drawCommandBuffer->beginCommandBuffer(currentCommandbuffer);

	// begin renderpass
	VkClearValue clearColor = { 1.0f, 0.0f, 1.0f, 1.0f };
	std::array<VkClearValue, 1> clearValues = {clearColor};
	renderPass->beginRenderPass(clearValues, 
							currentCommandbuffer,
							renderTexture->swapChainFramebuffers[imageIndex],
							renderTexture->_swapChainImageExtent);
}


void VulkanContext::drawEnd() {
	// end renderpass commands
	renderPass->endRenderPass(currentCommandbuffer);

	// end command buffer recording
	drawCommandBuffer->endCommandBuffer(currentCommandbuffer);

	// submit command buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &currentCommandbuffer;

	vkQueueSubmit(VulkanContext::getInstance()->getDevice()->graphicsQueue,
							1,
							&submitInfo,
							NULL);

	// present frame
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain->swapChain;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(VulkanContext::getInstance()->getDevice()->presentQueue,
					&presentInfo);
	vkQueueWaitIdle(VulkanContext::getInstance()->getDevice()->presentQueue);
}


void VulkanContext::cleanup() {
	vkDeviceWaitIdle(VulkanContext::getInstance()->getDevice()->logicalDevice);
	drawCommandBuffer->destroy();
	renderTexture->destroy();
	renderPass->destroy();
	swapChain->destroy();
	VulkanContext::getInstance()->getDevice()->destroy();
	valLayersAndExt->destroy(vInstance->vkInstance, isValidationLayersEnabled);
	vkDestroySurfaceKHR(vInstance->vkInstance, surface, nullptr);
	vkDestroyInstance(vInstance->vkInstance, nullptr);
	delete drawCommandBuffer;
	delete renderTexture;
	delete renderPass;
	delete swapChain;
	delete device;
	delete valLayersAndExt;
	delete vInstance;
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}
