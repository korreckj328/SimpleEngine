#include "Tools.h"
#include "VulkanContext.h"
#include <stdexcept>

namespace vkTools {
	VkImageView createImageView(VkImage image,
								VkFormat format,
								VkImageAspectFlags aspectFlags) {
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(VulkanContext::getInstance()->getDevice()->logicalDevice,
								&viewInfo,
								nullptr,
								&imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view");
		}
		return imageView;
	}

	void createBuffer(VkDeviceSize size,
					VkBufferUsageFlags usage,
					VkMemoryPropertyFlags properties,
					VkBuffer &buffer,
					VkDeviceMemory &bufferMemory) {
		VkBufferCreateInfo bufferInfo = {};

		bufferInfo.sType =VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice,
							&bufferInfo,
							nullptr,
							&buffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to create vertex buffer");
		}

		VkMemoryRequirements memrequirements;
		vkGetBufferMemoryRequirements(VulkanContext::getInstance()->getDevice()->logicalDevice,
									buffer,
									&memrequirements);
		
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memrequirements.size;
		allocInfo.memoryTypeIndex = findMemoryTypeIndex(memrequirements.memoryTypeBits,
									properties);

		if (vkAllocateMemory(VulkanContext::getInstance()->getDevice()->logicalDevice,
							&allocInfo,
							nullptr,
							&bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vert buffer mem\n");
		}
		vkBindBufferMemory(VulkanContext::getInstance()->getDevice()->logicalDevice,
							buffer,
							bufferMemory,
							0);
	}

	uint32_t findMemoryTypeIndex(uint32_t typeFilter,
								VkMemoryPropertyFlags properties) {
		// two arrays memory types and memory heaps
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(
				VulkanContext::getInstance()->getDevice()->physicalDevice, 
				&memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if((typeFilter & (1 << i)) && 
					(memProperties.memoryTypes[i].propertyFlags & properties)
					== properties) {
				return i;
			}
		}
		throw std::runtime_error("failed to find suitable memory type\n");
	}


	VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool) {
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(VulkanContext::getInstance()->getDevice()->logicalDevice,
								&allocInfo,
								&commandBuffer);
		// record command buffer
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		// start recording
		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}


	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool) {
		// end recording
		vkEndCommandBuffer(commandBuffer);

		// execute the command buffer
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(VulkanContext::getInstance()->getDevice()->graphicsQueue,
					1,
					&submitInfo,
					VK_NULL_HANDLE);
		vkQueueWaitIdle(VulkanContext::getInstance()->getDevice()->graphicsQueue);
		vkFreeCommandBuffers(VulkanContext::getInstance()->getDevice()->logicalDevice,
							commandPool,
							1,
							&commandBuffer);
	}


	void copyBuffer(VkBuffer srcBuffer,
					VkBuffer dstBuffer,
					VkDeviceSize size) {
		QueueFamilyIndices qFamilyIndices = 
			VulkanContext::getInstance()->getDevice()->getQueueFamiliesIndicesOfCurrentDevice();

		// create command pool
		VkCommandPool commandPool;
		VkCommandPoolCreateInfo cpInfo = {};
		cpInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cpInfo.queueFamilyIndex = qFamilyIndices.graphicsFamily;
		cpInfo.flags = 0;

		if (vkCreateCommandPool(VulkanContext::getInstance()->getDevice()->logicalDevice,
								&cpInfo,
								nullptr,
								&commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool\n");
		}
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

		VkBufferCopy copyregion = {};
		copyregion.srcOffset = 0;
		copyregion.dstOffset = 0;
		copyregion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyregion);
		endSingleTimeCommands(commandBuffer, commandPool);
		vkDestroyCommandPool(VulkanContext::getInstance()->getDevice()->logicalDevice,
							commandPool,
							nullptr);
	}
}
