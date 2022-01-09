#!/bin/env python

# use the env functions and pkg config
environment = Environment()
environment['CPPPATH'] = ['/lib']
environment.ParseConfig("pkg-config vulkan glfw3 glm --cflags --libs")

environment.Append(CPPDEFINES = ['DEBUG'])

# Set project name for binary
project_name = "Simple_Engine"


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
environment.Program(project_name, source=project_files)

