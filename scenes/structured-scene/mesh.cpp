#include "mesh.h"

Mesh::Mesh() :
    m_vaoID(0),
    m_vboID(0),
    m_iboID(0),
    m_indexCount(0)
{
}

void Mesh::createMesh(GLfloat *vertices, 
        unsigned int* indices,
        unsigned int numberOfVertices,
        unsigned int numberOfIndices)
{
    // Index count is needed later while 
    // drawing the mesh
    m_indexCount = numberOfIndices;

    // Specify a VAO for the mesh
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

        // Specify a VBO to pass in the index data
        // which will be used for index drawing
        glGenBuffers(1, &m_iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

        // Specify a VBO to bind to the above VAO
        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

            // Loading up the data of the tetrahedron into the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

            // Setting up attribute pointer for shader access
            // Arguments: Layout location, number of components in the vertex position attribute, 
                // type of the attribute, normalise?, stride, offset
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (const void*)0);
            // Enables the vertex position attribute
                // array specified at index 0
            glEnableVertexAttribArray(0);

    // Unbinding the VAO
    glBindVertexArray(0);
        // Unbinding the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbinding the IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::renderMesh()
{
    glBindVertexArray(m_vaoID);
        // Perform the draw call to initialise the rendering pipeline.
        // Arguments: drawing mode, number of indices, type of the index data, 
            // pointer to indices (0 because the data is already bound to IBO)
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::clearMesh()
{
    // To free VBO and IBO buffers use glDeleteBuffers()
    if(m_iboID)
    {
        glDeleteBuffers(1, &m_iboID);
        m_iboID = 0;
    }

    if(m_vboID)
    {
        glDeleteBuffers(1, &m_vboID);
        m_vboID = 0;
    }

    // To free a VAO use glDeleteVertexArrays()
    if(m_vaoID)
    {
        glDeleteVertexArrays(1, &m_vaoID);
        m_vaoID = 0;
    }

    m_indexCount = 0;
}

Mesh::~Mesh()
{
    // Clears the buffers in graphics card's memory
    // before destroying the mesh object.
    // Otherwise it could lead to memory leaks 
    // in the graphics card.
    clearMesh();
}