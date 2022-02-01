//
//  VulkanContext.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#ifndef VulkanContext_hpp
#define VulkanContext_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include "AppValidationLayersAndExtensions.h"
#include "VulkanInstance.h"
#include "Device.h"
#include "SwapChain.h"
#include "Renderpass.h"
#include "RenderTexture.h"
#include "DrawCommandBuffer.h"

#ifdef DEBUG
const bool isValidationLayersEnabled = true;
#else
const bool isValidationLayersEnabled = false;
#endif

class VulkanContext {
public:
    static VulkanContext* instance;
    static VulkanContext* getInstance();
    
    ~VulkanContext();
    void initVulkan(GLFWwindow *window);

	void drawBegin();
	void drawEnd();
	void cleanup();

    Device *getDevice();

	SwapChain *getSwapChain();
	Renderpass *getRenderpass();
	VkCommandBuffer getCurrentCommandBuffer();

private:
    AppValidationLayersAndExtensions *valLayersAndExt;
    VulkanInstance *vInstance;
    Device *device;
    SwapChain *swapChain;
	Renderpass *renderPass;
	RenderTexture *renderTexture;
	DrawCommandBuffer *drawCommandBuffer;

	uint32_t imageIndex = 0;
	VkCommandBuffer currentCommandbuffer;

    // Surface
    VkSurfaceKHR surface;
	const size_t MAX_FRAMES_IN_FLIGHT = 2;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	std::vector<VkFence> inFlightFences;
};

#endif /* VulkanContext_hpp */
