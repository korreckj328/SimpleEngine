//
//  Device.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 1/1/22.
//

#include "Device.h"

Device::Device() {
    
}

Device::~Device() {
    
}

void Device::pickPhysicalDevice(VulkanInstance *vInstance, VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vInstance->vkInstance, &deviceCount, nullptr);
    
    if(deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU's with Vulkan Support! \n");
    }
    
    std::cout << "Device Count: " << deviceCount << std::endl;
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vInstance->vkInstance, &deviceCount, devices.data());
    
    std::cout << std::endl;
    std::cout << "Device Properties" << std::endl;
    std::cout << "=================" << std::endl;
    for (const auto &device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);\
        std::cout << std::endl;
        std::cout << "Device name: " << deviceProperties.deviceName << std::endl;
        
        if(isDeviceSuitable(device, surface)) {
            physicalDevice = device;
            break;
        }
        if(physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find suitable gpu!\n");
        }
    }
}


bool Device::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    // find the queue families the device supports
    
    QueueFamilyIndices qFamilyIndicees = findQueueFamilies(device, surface);
    
    // check device extensions supported
    
    bool extensionsSupported = checkDeviceExtensionSupported(device);
    
    bool swapChainAdequate = false;
    
    if (extensionsSupported) {
        //check if surface formats and presentation modes are supported
        swapchainSupport = querySwapChainSupport(device, surface);
        swapChainAdequate = !swapchainSupport.surfaceFormat.empty() && !swapchainSupport.presentModes.empty();
    }
    
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    return qFamilyIndicees.arePresent() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.graphicsFamily = i;
        }
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        
        if(queueFamily.queueCount > 0 && presentSupport) {
            queueFamilyIndices.presentFamily = i;
        }
        
        if(queueFamilyIndices.arePresent()) {
            break;
        }
        i++;
    }
    
    return queueFamilyIndices;
}


bool Device::checkDeviceExtensionSupported(VkPhysicalDevice device) {
    uint32_t extensionCount;
    
    // get the number of available device extensions
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    
    // get available device extensions
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    // populate a set with required device extensions we need
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    
    // check that we have the required extensions
    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    
    return requiredExtensions.empty();
}

SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.surfaceCapabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    
    if(formatCount != 0) {
        details.surfaceFormat.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.surfaceFormat.data());
    }
    
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    
    if(presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
}

QueueFamilyIndices Device::getQueueFamiliesIndicesOfCurrentDevice() {
    return queueFamilyIndices;
}

void Device::createLogicalDevice(VkSurfaceKHR surface, bool isValidationLayerEnabled, AppValidationLayersAndExtensions *appValLayersAndExtensions) {
    
    
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
    
    float queuePriority = 1.0f;
    
    for(int queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

#ifdef __APPLE__
    // This needs to be added to support MoltenVK it seems.
    deviceExtensions.push_back("VK_KHR_portability_subset");
#endif

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    if(isValidationLayerEnabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(appValLayersAndExtensions->requiredValidationLayers.size());
        createInfo.ppEnabledLayerNames = appValLayersAndExtensions->requiredValidationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }
    
    // create logical device
    
    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device! \n");
    }
    
    // get handle to the graphics queue
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
    
    // get handle to the presentation queue
    vkGetDeviceQueue(logicalDevice, indices.presentFamily, 0, &presentQueue);
    
    
}
