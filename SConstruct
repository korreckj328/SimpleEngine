#!/bin/env python

import os

# use the env functions and pkg config
environment = Environment()


# currently builds on apple silicon macs and linux,  everything else is untested.
# mac install paths are from a downloaded sdk install from lunarg and a brew install
# of everything else.

if (environment['PLATFORM'] == 'darwin'):
    environment['CPPPATH'] = ['/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/include',
                                '/opt/homebrew/include']
    
    environment['LIBPATH'] = ['/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/lib', '/opt/homebrew/lib']
    
    environment.Append(LIBS = ['libvulkan.1.2.198.dylib', 'libglfw.3.3.dylib'])
    print(environment['LIBS'])


    # environment.Append(LINKFLAGS = ['-install_name @/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/lib/libvulkan.1.2.198.dylib', '-install_name @/opt/homebrew/lib/libglfw.3.3.dylib'])
    
    environment['CCFLAGS'] = ['-std=c++11']
else:
    environment['CPPPATH'] = ['/lib']
    environment.ParseConfig("pkg-config vulkan glfw3 glm --cflags --libs")

    environment.Append(CPPDEFINES = ['DEBUG'])

    x11_error = os.system("pkg-config --version > /dev/null")
    if x11_error:
        print("Error: pkg-config not found. Aborting.")
        exit(-1)

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

