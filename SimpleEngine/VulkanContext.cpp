//
//  VulkanContext.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "VulkanContext.h"
#include <array>
#include <limits>
#include <stdexcept>

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


	// synchronization
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(device->logicalDevice, 
					&semaphoreInfo, 
					nullptr, 
					&imageAvailableSemaphore);

	vkCreateSemaphore(device->logicalDevice,
					&semaphoreInfo,
					nullptr,
					&renderFinishedSemaphore);

	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if(vkCreateFence(device->logicalDevice,
						&fenceCreateInfo,
						nullptr,
						&inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create Fence per frame");
		}
	}
}

Device *VulkanContext::getDevice() {
    return device;
}


void VulkanContext::drawBegin() {
	vkAcquireNextImageKHR(VulkanContext::getInstance()->getDevice()->logicalDevice,
					swapChain->swapChain,
					std::numeric_limits<uint64_t>::max(),
					imageAvailableSemaphore,
					VK_NULL_HANDLE,
					&imageIndex);
	
	vkWaitForFences(device->logicalDevice,
					1,
					&inFlightFences[imageIndex],
					VK_TRUE,
					std::numeric_limits<uint64_t>::max());
	
	vkResetFences(device->logicalDevice,
					1,
					&inFlightFences[imageIndex]);


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

	// wait for the stage that writes to color attachment
	VkPipelineStageFlags waitStages[] = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};

	// which stage of the pipeline to wait 
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

	vkQueueSubmit(VulkanContext::getInstance()->getDevice()->graphicsQueue,
							1,
							&submitInfo,
							inFlightFences[imageIndex]);

	// present frame
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain->swapChain;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(VulkanContext::getInstance()->getDevice()->presentQueue,
					&presentInfo);
	vkQueueWaitIdle(VulkanContext::getInstance()->getDevice()->presentQueue);
}


void VulkanContext::cleanup() {
	vkDeviceWaitIdle(VulkanContext::getInstance()->getDevice()->logicalDevice);

	vkDestroySemaphore(device->logicalDevice, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device->logicalDevice, imageAvailableSemaphore, nullptr);

	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyFence(device->logicalDevice, inFlightFences[i], nullptr);
	}

	drawCommandBuffer->destroy();
	renderTexture->destroy();
	renderPass->destroy();
	swapChain->destroy();
	device->destroy();
	valLayersAndExt->destroy(vInstance->vkInstance, isValidationLayersEnabled);
	vkDestroySurfaceKHR(vInstance->vkInstance, surface, nullptr);
	vkDestroyInstance(vInstance->vkInstance, nullptr);
	delete drawCommandBuffer;
	delete renderTexture;
	delete renderPass;
	// delete swapChain;
	delete device;
	delete valLayersAndExt;
	delete vInstance;
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}


SwapChain *VulkanContext::getSwapChain() {
	return swapChain;
}


Renderpass *VulkanContext::getRenderpass() {
	return renderPass;
}


VkCommandBuffer VulkanContext::getCurrentCommandBuffer() {
	return currentCommandbuffer;
}
