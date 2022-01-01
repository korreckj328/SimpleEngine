//
//  VulkanInstance.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "VulkanInstance.hpp"

VulkanInstance::VulkanInstance() {
    
}

VulkanInstance::~VulkanInstance() {
    std::cout << "Vulkan Instance Destroyed" << std::endl;
}

void VulkanInstance::createAppAndVkInstance(bool enableValidationLayers, AppValidationLayersAndExtensions *valLayersAndExtensions) {
      
    // links the app to the vulkan lib
    
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Simple Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "SimpleEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    
    const char* instExtension = "VK_KHR_get_physical_device_properties2";
    
    VkInstanceCreateInfo vkInstanceInfo = {};
    vkInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vkInstanceInfo.pApplicationInfo = &appInfo;
    
    // specify extensions and validation layers
    auto extensions = valLayersAndExtensions->getRequiredExtensions(enableValidationLayers);
    
    vkInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size() + 1);
    extensions.push_back(instExtension);
    vkInstanceInfo.ppEnabledExtensionNames = extensions.data();
    
    if(enableValidationLayers) {
        vkInstanceInfo.enabledLayerCount = static_cast<uint32_t>(valLayersAndExtensions->requiredValidationLayers.size());
        vkInstanceInfo.ppEnabledLayerNames = valLayersAndExtensions->requiredValidationLayers.data();
    } else {
        vkInstanceInfo.enabledLayerCount = 0;
    }
    if(vkCreateInstance(&vkInstanceInfo, nullptr, &vkInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vkInstance \n");
    }
}
