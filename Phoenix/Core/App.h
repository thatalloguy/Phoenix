//
// Created by allos on 04/11/2025.
//
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Phoenix {


    struct WindowCreationInfo {
        const char* name = "Phoenix Application";
        int width = 1280, height = 720;
    };


    class App {
    public:
        App() = default;

        virtual ~App() = default;

        virtual void Initialize() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

        virtual bool ShouldClose() { return glfwWindowShouldClose(_window); }

    protected:
        void InitializeWindow(const WindowCreationInfo& info);

        GLFWwindow* _window = nullptr;
    };
}
