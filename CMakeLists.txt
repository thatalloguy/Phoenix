cmake_minimum_required(VERSION 3.31)
project(Phoenix)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("Thirdparty/glfw")
add_subdirectory("Thirdparty/glm")

file(GLOB_RECURSE THIRDPARTY_SRC
        Thirdparty/stb_image.h
        Thirdparty/glad/src/*.c
)

file(GLOB_RECURSE ENGINE_SRC
        Phoenix/Core/*.h
        Phoenix/Core/*.cpp
)

file(GLOB_RECURSE GAME_SRC
        Game/Core/*.cpp
        Game/Core/*.h
        Game/*.cpp
)

include_directories("Thirdparty/glad/include")
include_directories("Thirdparty/glfw/include")
include_directories("Thirdparty/glm")

add_executable(Phoenix Game/main.cpp ${THIRDPARTY_SRC} ${ENGINE_SRC} ${GAME_SRC})

target_link_libraries(Phoenix PUBLIC glm)
target_link_libraries(Phoenix PUBLIC glfw)

