//
// Created by allos on 04/11/2025.
//

#include "Game.h"

void Game::Initialize() {
    App::InitializeWindow(Phoenix::WindowCreationInfo{
        "HP SS remake",
        1280, 720
    });
}

void Game::Update(float deltaTime) {
    glfwSwapBuffers(_window);
    glfwPollEvents();


}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2, 0.3, 0.3, 1);
}

void Game::Shutdown() {
    glfwTerminate();
}
