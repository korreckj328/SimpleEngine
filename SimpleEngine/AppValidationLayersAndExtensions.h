//
//  AppValidationLayersAndExtensions.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#ifndef AppValidationLayersAndExtensions_hpp
#define AppValidationLayersAndExtensions_hpp

#include <stdio.h>

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <cstring>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN ;
#endif

#include <GLFW/glfw3.h>

class AppValidationLayersAndExtensions {
public:
    AppValidationLayersAndExtensions();
    ~AppValidationLayersAndExtensions();
    
    const std::vector<const char*> requiredValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions(bool isValidationLayersEnabled);
    
    VkDebugReportCallbackEXT callback;
    
    void setupDebugCallback(bool isValidationLayersEnabled, VkInstance vkInstance);
    void destroy(VkInstance instance, bool isValidationLayersEnabled);
    
    VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

    void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
};


#endif /* AppValidationLayersAndExtensions_hpp */
