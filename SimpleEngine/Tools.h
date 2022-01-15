#ifndef TOOLS_H
#define TOOLS_H

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

namespace vkTools {
	VkImageView createImageView(VkImage image, 
								VkFormat format, 
								VkImageAspectFlags aspectFlags);
}

#endif
