#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include<vulkan/vulkan.h>
#include<vector>
#include<fstream>


class GraphicsPipeline {
public:
	GraphicsPipeline();
	~GraphicsPipeline();

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	void createGraphicsPipelineAndLayout(VkExtent2D swapChainImageExtent,
										VkDescriptorSetLayout descriptorSetLayout,
										VkRenderPass renderPass);

	void destroy();

private:
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;
	std::vector<char> readfile(const std::string &filename);
	VkShaderModule createShaderModule(const std::vector<char> &code);
	void createGraphicsPipelineLayout(VkDescriptorSetLayout descriptorSetLayout);
	void createGraphicsPipeline(VkExtent2D swapChainImageExtent, VkRenderPass renderPass);
};

#endif
