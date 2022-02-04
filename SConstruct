#!/bin/env python

import os
import subprocess

# use the env functions and pkg config
environment = Environment(COMPILATIONDB_USE_ABSPATH=True)
environment.Tool('compilation_db')
environment.CompilationDatabase()
environment.Append(CPPDEFINES = ['DEBUG'])
if(environment['PLATFORM'] != 'win32'):
    environment['CCFLAGS'] = ['-std=c++11', '-g', '-Wall']


# currently builds on apple silicon macs and linux,  everything else is untested.
# mac install paths are from a downloaded sdk install from lunarg and a brew install
# of everything else.

def compile_shaders():
    print("About to compile shaders.")
    if (environment['PLATFORM'] == 'darwin'):
        compiler = "/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/bin/glslangValidator"
    else:
        compiler = "glslangValidator"
    print("Compiler: ", compiler)
    shaders = [
                "Shaders/basic_fragment.frag",
                "Shaders/basic_vertex.vert",
    ]

    print("Shaders: ", shaders)
    for shader in shaders:
        print("compiling shader: ", shader)
        my_out = 'SPIRV/'
        target = os.path.join(my_out, os.path.basename(shader) + '.spv')
        subprocess.call([compiler, '-V', shader, '-o', target])

if (environment['PLATFORM'] == 'darwin'):
    environment['CPPPATH'] = [
            '/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/include',
            '/opt/homebrew/include'
    ]
    
    environment['LIBPATH'] = [
            '/Users/jeremiahkorreck/VulkanSDK/1.2.198.1/macOS/lib', 
            '/opt/homebrew/lib'
    ]
    
    environment.Append(LIBS = [
            'libvulkan.1.2.198.dylib', 
            'libglfw.3.3.dylib'
    ])

    print(environment['LIBS'])

elif (environment['PLATFORM'] == 'win32'):
    print("In platform win32")
    environment['CPPPATH'] = [
            "C:\\VulkanSDK\\1.2.198.1\\Include",
            "C:\\Libraries\\g-truc-glm-bf71a83",
            "C:\\Libraries\\glfw-3.3.6.bin.WIN64\\include"
    ]
    print(environment['CPPPATH'])
    environment['LIBPATH'] = [
            "C:\\Libraries\\glfw-3.3.6.bin.WIN64\\lib-vc2022",
            "C:\\VulkanSDK\\1.2.198\\Lib32"
    ]
    environment.Append(LIBS = [
            "glfw3",
            "vulkan"
    ])
    print(environment['LIBPATH'])
    print(environment['LIBS'])
else: 
    print(environment['PLATFORM'])
    environment.ParseConfig("pkg-config vulkan glfw3 glm --cflags --libs")

    x11_error = os.system("pkg-config --version > /dev/null")
    if x11_error:
        print("Error: pkg-config not found. Aborting.")
        exit(-1)

environment.Append(CPPPATH = 'stb')

# Set project name for binary
project_name = "Simple_Engine"


# Set my source files to a list.   


project_files = Glob('SimpleEngine/*.cpp')

if ARGUMENTS.get('compile_shaders', 'no') == 'yes':
    compile_shaders()
    print("compile shaders ran")




# Set the program to create
environment.Program(project_name, source=project_files)

