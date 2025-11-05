//
// Created by allos on 05/11/2025.
//
#pragma once

#include "assimp/material.h"
#include <vector>
#include <glm.hpp>

#include "Shader.h"
#include "Texture.h"

struct aiMaterial;
//forward declares so that assimp isnt included.
struct aiMesh;
struct aiScene;
struct aiNode;


namespace Phoenix {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    class Mesh {
    public:

        Mesh() = default;
        Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& ins, const std::vector<std::pair<std::string, Texture*>>& texs) {
            vertices = verts;
            indices = ins;
            textures = texs;
        }

        void Init();
        void Draw(glm::mat4& parent_transform, Shader& shader, int texture_slot_offset=0);
        void Destroy();

        void SetOffset(const glm::mat4& m) { offset = m;}
    private:
        glm::mat4 offset{};

        unsigned int VAO{0};
        unsigned int VBO{0};
        unsigned int EBO{0};

        std::vector<Vertex> vertices{};
        std::vector<unsigned int> indices{};
        std::vector<std::pair<std::string, Texture*>> textures{};
    };


    class Model {
    public:
        Model() = default;
        ~Model() {
            Destroy();
        }

        void Load(const std::string& path);
        void Draw(Shader& shader, int texture_slot_offset=0);
        void Destroy();

        void SetTransform(const glm::mat4& matrix ) { _transform = matrix;};
        const glm::mat4& GetTransform() const { return _transform; }

    private:
        glm::mat4 _transform{};
        std::vector<Mesh> _meshes;

        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, aiMatrix4x4& parentMat, const aiScene* scene);
        std::vector<std::pair<std::string, Texture*>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name);

    };


}