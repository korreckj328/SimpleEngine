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
    
}


void Application::run() {
    mainLoop();
}

void Application::mainLoop() {
    std::cout << "Entering main loop" << std::endl;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    std::cout << "Exiting main loop" << std::endl;
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Application Destroyed" << std::endl;
}
