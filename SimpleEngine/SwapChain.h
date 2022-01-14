//
//  SwapChain.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 1/3/22.
//

#ifndef SwapChain_hpp
#define SwapChain_hpp

#include <stdio.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>

class SwapChain {
public:
    SwapChain();
    ~SwapChain();
    
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainImageExtent;
    std::vector<VkImage> swapChainImages;
    
    VkSurfaceFormatKHR chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    
    void create(VkSurfaceKHR surface);
    void destroy();
};

#endif /* SwapChain_hpp */
