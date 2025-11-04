#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Game.h"


int main() {

    Game game{};

    game.Initialize();

    while (!game.ShouldClose()) {
        game.Update(0.0f);
        game.Render();
    }

    return 0;
}
