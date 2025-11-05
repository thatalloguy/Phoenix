//
// Created by allos on 05/11/2025.
//
#pragma once
#include "glad/glad.h"
#include <glm.hpp>

namespace Phoenix {
    typedef unsigned int ShaderProgram;

    /// Statuses for shader operations
    enum class ShaderStatus: unsigned int {
        SUCCESSFUL = 0,

        UNKNOWN_SOURCE = 1,
        COMPILATION_FAILED = 2,
    };

    /// Shader Type (more to be added in the future such as geometry).
    enum ShaderType: unsigned int {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
    };

    struct ShaderCreateInfo {
        const char* vertexSRC = nullptr;
        const char* fragmentSRC = nullptr;
        const char* geometrySRC = nullptr;
    };

    /*
     * A Shader is a object that holds a program that runs on the GPU.
     */
    class Shader {

    public:
        Shader();
        ~Shader();

        /// Creates 2 shaders for the Vertex and Fragment Shader
        void Init(const ShaderCreateInfo& createInfo);

        /// Adds a new subShader to the ShaderProgram
        ShaderStatus AddSubShader(const char* src, ShaderType type);

        /// Returns the last compilation error.
        const char* GetCompilationErrorMessage();

        /// Returns the raw ShaderProgram object for GL uses
        ShaderProgram GetShaderProgram() { return shaderProgram; };

        /// Binds the shader
        void Bind();

        /// Sets a int uniform
        void SetInt(const char* name, int val);
        void SetFloat(const char* name, float val);
        void SetTransform(const char* name,float* val);

        void SetVector3(const char* name, const glm::vec3& vec);

    private:

        ShaderProgram LoadShader(const char* src, ShaderType type);

        ShaderProgram shaderProgram;

    };

}
