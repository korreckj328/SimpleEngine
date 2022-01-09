#!/bin/env python

# use the env functions and pkg config
enviroment = Environment()
enviroment['CPPPATH'] = ['/lib']
enviroment.ParseConfig("pkg-config vulkan glfw3 glm --cflags --libs")
print(enviroment['CPPPATH'])


# Set project name for binary
project_name = "SimpleEngine"


# Set my source files to a list.  Appending on one by one rather than using a glob for 
# clarity

project_files = []
project_files.append("SimpleEngine/main.cpp")
project_files.append("SimpleEngine/Application.cpp")
project_files.append("SimpleEngine/AppValidationLayersAndExtensions.cpp")
project_files.append("SimpleEngine/Device.cpp")
project_files.append("SimpleEngine/SwapChain.cpp")
project_files.append("SimpleEngine/VulkanContext.cpp")
project_files.append("SimpleEngine/VulkanInstance.cpp")


# Set the program to create
enviroment.Program(source=project_files)

