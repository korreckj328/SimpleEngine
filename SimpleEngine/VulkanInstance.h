//
//  VulkanInstance.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#ifndef VulkanInstance_hpp
#define VulkanInstance_hpp

#include <stdio.h>
#include <vulkan/vulkan.h>

#include "AppValidationLayersAndExtensions.h"

class VulkanInstance {
public:
    VulkanInstance();
    ~VulkanInstance();
    
    VkInstance vkInstance;
    
    void createAppAndVkInstance(bool enableValidationLayers, AppValidationLayersAndExtensions *valLayersAndExtensions);
    
};

#endif /* VulkanInstance_hpp */
