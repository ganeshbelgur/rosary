#pragma once

#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    void createMesh(GLfloat *vertices, 
        unsigned int* indices,
        unsigned int numberOfVertices,
        unsigned int numberOfIndices);
    void renderMesh();
    void clearMesh();
    ~Mesh();

private:
    GLuint m_vaoID, m_vboID, m_iboID;
    GLsizei m_indexCount;
};