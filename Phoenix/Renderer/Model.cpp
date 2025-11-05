//
// Created by allos on 05/11/2025.
//

#include "Model.h"

#include "glad/glad.h"

#include <gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Phoenix::Mesh::Init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // tex_coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Phoenix::Mesh::Draw(glm::mat4 &parent_transform, Shader &shader, int texture_slot_offset) {
    shader.Bind();

    //Bind textures
    for (int i=0; i<textures.size(); i++) {
        std::pair<std::string, Texture*>& pair = textures[i];

        pair.second->bind(texture_slot_offset + i);
        shader.SetInt(pair.first.c_str(), texture_slot_offset + i);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Phoenix::Mesh::Destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (std::pair<std::string, Texture*>& pair : textures) {
        delete pair.second;
    }
}

void Phoenix::Model::Load(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("Assimp error while load: [%s] | %s", path.c_str(), import.GetErrorString());
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Phoenix::Model::Draw(Shader &shader, int texture_slot_offset) {
    shader.Bind();
    shader.SetTransform("transform", glm::value_ptr(_transform));
    for (Mesh& mesh : _meshes) {
        mesh.Draw(_transform, shader, texture_slot_offset);
    }
}

void Phoenix::Model::Destroy() {
    for (Mesh& mesh : _meshes) {
        mesh.Destroy();
    }
}

void Phoenix::Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        aiMatrix4x4 offset_matrix = node->mTransformation;
        if (node->mParent) {
            // offset_matrix = node->mParent->mTransformation * node->mTransformation;
        }


        Mesh raw_mesh = ProcessMesh(mesh, offset_matrix, scene);
        raw_mesh.Init();
        _meshes.push_back(raw_mesh);
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Phoenix::Mesh Phoenix::Model::ProcessMesh(aiMesh *mesh, aiMatrix4x4 &parentMat, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::pair<std::string, Texture*>> textures;


    //load vertices
    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
        Vertex vertex;
        aiVector3D vec = parentMat * mesh->mVertices[i];

        vertex.position.x = vec.x;
        vertex.position.y = vec.y;
        vertex.position.z = vec.z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.uv = glm::vec2{0, 0};
        }

        vertices.push_back(vertex);
    }

    //load indices
    for (unsigned int i=0; i< mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j=0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //load materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");

    }

    return Mesh{vertices, indices, textures};
}

std::vector<std::pair<std::string, Phoenix::Texture*>> Phoenix::Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
    const std::string &type_name) {
    std::vector<std::pair<std::string, Texture*>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture* texture = new Texture{};
        texture->init(TextureCreationInfo{
            .source = TextureSource{
                .path = str.C_Str(),
            },
        });
        textures.emplace_back(type_name, texture);
    }
    return textures;
}
