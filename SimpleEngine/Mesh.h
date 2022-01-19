#ifndef MESH_H
#define MESH_H

#include <vulkan/vulkan.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MeshType {
	kTriangle = 0,
	kQuad = 1,
	kCube = 2,
	kSphere =3
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0; // binding location index
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getAttribDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4> attribDescriptions = {};
		
		attribDescriptions[0].binding = 0;
		attribDescriptions[0].location = 0;
		attribDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDescriptions[0].offset = offsetof(Vertex, pos);
		
		attribDescriptions[1].binding = 0;
		attribDescriptions[1].location = 1;
		attribDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDescriptions[1].offset = offsetof(Vertex, normal);

		attribDescriptions[2].binding = 0;
		attribDescriptions[2].location = 2;
		attribDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDescriptions[2].offset = offsetof(Vertex, color);

		attribDescriptions[3].binding = 0;
		attribDescriptions[3].location = 3;
		attribDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attribDescriptions[3].offset = offsetof(Vertex, texCoords);

		return attribDescriptions;
	}
};

class Mesh {
public:
	static void setTriData(std::vector<Vertex> &vertices, 
							std::vector<uint32_t> &indices);

	static void setQuadData(std::vector<Vertex> &vertices,
							std::vector<uint32_t> &indices);

	static void setCubeData(std::vector<Vertex> &vertices,
							std::vector<uint32_t> &indices);

	static void setSphereData(std::vector<Vertex> &vertices,
							std::vector<uint32_t> &indices);
};

#endif
