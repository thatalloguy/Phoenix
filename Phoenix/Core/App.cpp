//
// Created by allos on 04/11/2025.
//

#include "App.h"
#include <cstdio>


void Phoenix::App::InitializeWindow(const WindowCreationInfo& info) {
    if (!glfwInit()) {
        printf("[Error] Could not initailize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _window = glfwCreateWindow(info.width, info.height, info.name, NULL, NULL);
    if (!_window) {
        glfwTerminate();
        printf("[Error] Could not create GLFW window\n");
        return;
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("[Error] Failed to initialize Opengl\n");
        return;
    }



}
