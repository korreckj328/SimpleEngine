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

	object.createObjectRenderer(MeshType::kTriangle, 
								glm::vec3(-1.0f, 1.0f, 0.0f), 
								glm::vec3(0.5f));
}


void Application::run() {
    mainLoop();
}

void Application::mainLoop() {
    std::cout << "Entering main loop" << std::endl;
    while (!glfwWindowShouldClose(window)) {
		VulkanContext::getInstance()->drawBegin();

		object.updateUniformbuffer(camera);
		object.draw();


		VulkanContext::getInstance()->drawEnd();
		glfwPollEvents();
    }
    std::cout << "Exiting main loop" << std::endl;
}

Application::~Application() {
	object.destroy();
	VulkanContext::getInstance()->cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Application Destroyed" << std::endl;
}
