//
//  Device.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 1/1/22.
//

#ifndef Device_hpp
#define Device_hpp

#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>

#include "VulkanInstance.hpp"
#include "AppValidationLayersAndExtensions.hpp"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> surfaceFormat;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;
    bool arePresent() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

class Device {
    // TODO figure out what can be private here
public:
    Device();
    ~Device();
    
    // physical device setup
    
    VkPhysicalDevice physicalDevice;
    SwapChainSupportDetails swapchainSupport;
    QueueFamilyIndices queueFamilyIndices;
    
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    void pickPhysicalDevice(VulkanInstance *vInstance, VkSurfaceKHR surface);
    
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupported(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    
    QueueFamilyIndices getQueueFamiliesIndicesOfCurrentDevice();
    
    // logical device setup
    
    void createLogicalDevice(VkSurfaceKHR surface, bool isValidationLayerEnabled, AppValidationLayersAndExtensions *appValLayersAndExtensions);
    
    VkDevice logicalDevice;
    
    // handle to the graphics queue
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
    void destroy();
};
#endif /* Device_hpp */
