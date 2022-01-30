//
//  Application.hpp
//  SimpleEngine
//
//  Created by Jeremiah Korreck on 12/31/21.
//

#ifndef Application_hpp
#define Application_hpp


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>

#include "VulkanContext.h"
#include "SwapChain.h"
#include "Camera.h"
#include "ObjectRenderer.h"

class Application {
public:
    Application();
    ~Application();
    void run();
    
private:
    void mainLoop();
    
	Camera camera;
	ObjectRenderer object;

    GLFWwindow *window;
    VulkanContext *context;
    
    const uint32_t WIDTH = 1280;
    const uint32_t HEIGHT = 720;
    const std::string appName = "Simple Engine";
};





#endif /* Application_hpp */
