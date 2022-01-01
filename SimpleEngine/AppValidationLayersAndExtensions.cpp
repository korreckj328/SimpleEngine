//
//  AppValidationLayersAndExtensions.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "AppValidationLayersAndExtensions.hpp"
#include <iostream>

AppValidationLayersAndExtensions::AppValidationLayersAndExtensions() {
    
}

AppValidationLayersAndExtensions::~AppValidationLayersAndExtensions() {
    
}

bool AppValidationLayersAndExtensions::checkValidationLayerSupport() {
    uint32_t layerCount;
    
    // get count of available validation layers
    std::cout << "Getting layer count ";
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::cout << "done!" << std::endl;
    
    // get the names of the available validation layers
    std::cout << "Getting layer names ";
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    std::cout << "Done" << std::endl;
    std::cout << "Checking for validation layers \n";
    int count = 0;
    for (const char* layerName : requiredValidationLayers) {
        std::cout << requiredValidationLayers.size() << std::endl;
        std::cout << "Pass" << count << std::endl;
        count++;
        bool layerFound = false;
        int subCount = 0;
        for (const auto& layerproperties : availableLayers) {
            std::cout << "SubPass" << subCount << std::endl;
            subCount++;
            std::cout << layerproperties.layerName << std::endl;
            if(strcmp(layerName, layerproperties.layerName) == 0) {
                layerFound = true;
                std::cout << "Layer Found" << std::endl;
                break;
            }
        }
        if(!layerFound) {
            std::cout << "Layer not Found" << std::endl;
            return false;
        }
    }
    return true;
}

std::vector<const char*> AppValidationLayersAndExtensions::getRequiredExtensions(bool isValidationLayersEnabled) {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtenstions;
    
    glfwExtenstions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtenstions, glfwExtenstions + glfwExtensionCount);
    
    
    if(isValidationLayersEnabled) {
        extensions.push_back("VK_EXT_debug_report");
    }
    return extensions;
}

// setup callback function to print callback report
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objEXT, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
    std::cerr <<"Validation layer: " << msg << std::endl << std::endl;
    return false;
}

void AppValidationLayersAndExtensions::setupDebugCallback(bool isValidationLayersEnabled, VkInstance vkInstance) {
    if(!isValidationLayersEnabled) {
        return;
    }
    std::cout << "setup call back ";
    VkDebugReportCallbackCreateInfoEXT info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    info.pfnCallback = debugCallback;
    if(createDebugReportCallbackEXT(vkInstance, &info, nullptr, &callback) != VK_SUCCESS) {
        throw std::runtime_error("failed to set debug callback!");
    }
    std::cout << "Done!" << std::endl;
}

void AppValidationLayersAndExtensions::destroy(VkInstance instance, bool isValidationLayersEnabled) {
    if (isValidationLayersEnabled) {
        DestroyDebugReportCallbackEXT(instance, callback, nullptr);
    }
}

VkResult AppValidationLayersAndExtensions::createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

    if (func != nullptr) {
      return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void AppValidationLayersAndExtensions::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}
