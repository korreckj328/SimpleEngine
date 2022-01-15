//
//  VulkanContext.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "VulkanContext.h"

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
}

Device *VulkanContext::getDevice() {
    return device;
}
