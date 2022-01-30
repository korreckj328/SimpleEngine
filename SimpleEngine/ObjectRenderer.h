#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include <glm/vec3.hpp>

#include "GraphicsPipeline.h"
#include "ObjectBuffers.h"
#include "Descriptor.h"
#include "Camera.h"

class ObjectRenderer {
public:
	void createObjectRenderer(MeshType modelType, glm::vec3 _position, glm::vec3 _scale);
	void updateUniformbuffer(Camera camera);
	void draw();
	void destroy();
private:
	GraphicsPipeline gPipeline;
	ObjectBuffers objBuffers;
	Descriptor descriptor;
	glm::vec3 position;
	glm::vec3 scale;
};

#endif
