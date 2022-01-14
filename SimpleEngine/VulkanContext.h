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

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include "AppValidationLayersAndExtensions.hpp"
#include "VulkanInstance.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

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
    
    Device *getDevice();
private:
    AppValidationLayersAndExtensions *valLayersAndExt;
    VulkanInstance *vInstance;
    Device *device;
    SwapChain *swapChain;
    // Surface
    VkSurfaceKHR surface;
};

#endif /* VulkanContext_hpp */
