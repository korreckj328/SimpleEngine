//
//  Application.cpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#include "Application.h"
Application::Application() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, appName.c_str(), nullptr, nullptr);
    
    context = VulkanContext::getInstance();
    context->getInstance()->initVulkan(window);
	camera.init(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f);
	camera.setCameraPosition(glm::vec3(0.0f, 0.0f, 4.0f));


	ObjectRenderer triangle;
	triangle.createObjectRenderer(MeshType::kTriangle, 
								glm::vec3(-1.0f, 1.0f, 0.0f), 
								glm::vec3(0.5f));

	objects.push_back(triangle);

	ObjectRenderer quad;
	quad.createObjectRenderer(MeshType::kQuad,
							glm::vec3(1.0f, 1.0f, 0.0f),
							glm::vec3(0.5f));
	objects.push_back(quad);

	ObjectRenderer cube;
	cube.createObjectRenderer(MeshType::kCube,
							glm::vec3(-1.0f, -1.0f, 0.0f),
							glm::vec3(0.5f));
	objects.push_back(cube);

	ObjectRenderer sphere;
	sphere.createObjectRenderer(MeshType::kSphere,
								glm::vec3(1.0f, -1.0f, 0.0f),
								glm::vec3(0.5f));
	objects.push_back(sphere);
}


void Application::run() {
    mainLoop();
}

void Application::mainLoop() {
    std::cout << "Entering main loop" << std::endl;
    while (!glfwWindowShouldClose(window)) {
		VulkanContext::getInstance()->drawBegin();

		for (auto obj : objects) {
			obj.updateUniformbuffer(camera);
			obj.draw();
		}

		VulkanContext::getInstance()->drawEnd();
		glfwPollEvents();
    }
    std::cout << "Exiting main loop" << std::endl;
	std::cout << "Waiting for device ... ";
	vkDeviceWaitIdle(VulkanContext::getInstance()->getDevice()->logicalDevice);
	std::cout << "Done" << std::endl;
}

Application::~Application() {
	for (auto obj : objects) {
		obj.destroy();
	}
	VulkanContext::getInstance()->cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Application Destroyed" << std::endl;
}
