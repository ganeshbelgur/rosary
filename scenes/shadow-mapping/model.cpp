//
//  Rosary source code is Copyright(c) 2016-2020 Ganesh Belgur
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//  - Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  - Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "model.h"

Model::Model()
{
}

bool Model::loadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        fileName,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals|
        aiProcess_JoinIdenticalVertices);
    
    if (!scene)
    {
        printf("Error: Model %s failed to load: %s\n", fileName.c_str(), importer.GetErrorString());
        return false;
    }
    
    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);

    return true;
}

void Model::loadNode(aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for(size_t i = 0; i < node->mNumChildren; ++i)
    {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
        
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }

        // Find out why the normals need to be inverted
        vertices.insert(vertices.end(), {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z});
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh *newMesh = new Mesh();
    newMesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
    m_meshList.push_back(newMesh);
    m_meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene *scene)
{
    m_textureList.resize(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial *material = scene->mMaterials[i];
        m_textureList[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                int idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx + 1);
                std::string texturePath = std::string("./scenes/shadow-mapping/assets/textures/") + filename;
                m_textureList[i] = new Texture();
                m_textureList[i]->createTexture(texturePath.c_str());

                if (!m_textureList[i]->loadTextureRGBOnly())
                {
                    printf("Error: Model::loadMaterials(): Failed to load texture at %s\n", texturePath.c_str());
                    delete m_textureList[i];
                    m_textureList[i] = nullptr;
                }
            }
        }

        if (!m_textureList[i])
        {
            m_textureList[i] = new Texture();
            m_textureList[i]->createTexture("./scenes/shadow-mapping/assets/textures/plain.png");
            m_textureList[i]->loadTextureWithAlpha();
        }
    }
}

void Model::renderModel()
{
    for (size_t i = 0; i < m_meshList.size(); ++i)
    {
        unsigned int materialIndex = m_meshToTexture[i];

        if (materialIndex < m_textureList.size() && m_textureList[materialIndex])
        {
            m_textureList[materialIndex]->useTexture();
        }

        m_meshList[i]->renderMesh();
    }
}

void Model::clearModel()
{
    for (size_t i = 0; i < m_meshList.size(); ++i)
    {
        if (m_meshList[i])
        {
            delete m_meshList[i];
            m_meshList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < m_textureList.size(); ++i)
    {
        if (m_textureList[i])
        {
            delete m_textureList[i];
            m_textureList[i] = nullptr;
        }
    }
}

Model::~Model()
{
}
