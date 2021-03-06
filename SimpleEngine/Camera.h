#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIAN

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	void init(float FOV, float width, float height, float nearPlane, float farPlane);
	void setCameraPosition(glm::vec3 position);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
};

#endif

