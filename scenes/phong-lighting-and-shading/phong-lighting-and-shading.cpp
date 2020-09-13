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

#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window-manager.h"
#include "mesh.h"
#include "shader-manager.h"
#include "camera.h"
#include "texture.h"
#include "light.h"
#include "material.h"

// Scene data
WindowManager window;
std::vector<Mesh*> meshes;
std::vector<ShaderManager> shaderManagers;
Camera camera;
Light light;
Texture brickTexture;
Texture dirtTexture;
Material shinyMaterial;
Material dullMaterial;

// Path to the shader files relative to Rosary's Makefile
static const char* vertexShaderPath = "./scenes/phong-lighting-and-shading/vertex.glsl";
static const char* fragmentShaderPath = "./scenes/phong-lighting-and-shading/fragment.glsl";

// Average the normals at the vertex positions
// for all the triangular surfaces, no interpolation
// performed yet. Interpolation occurs in the shaders!
void calculateNormalsForPhongShading(
    GLfloat *vertices,
    unsigned int numOfVertices,
    unsigned int *indices, 
    unsigned int numOfIndices,
    unsigned int perVertexDataLength,
    unsigned int normalDataOffset)
{
    for(size_t i = 0; i < numOfIndices; i += 3)
    {
        // Find the starting location in the data array for 
        // the vertices making up the given triangluar surface
        unsigned int vertex0 = indices[i] * perVertexDataLength;
        unsigned int vertex1 = indices[i+1] * perVertexDataLength;
        unsigned int vertex2 = indices[i+2] * perVertexDataLength;

        // Calculate the vectors required to generate 
        // the normal vector for the particular trianglar face
        glm::vec3 sideVector1(
            vertices[vertex1] - vertices[vertex0],
            vertices[vertex1 + 1] - vertices[vertex0 + 1],
            vertices[vertex1 + 2] - vertices[vertex0 + 2]);

        glm::vec3 sideVector2(
            vertices[vertex2] - vertices[vertex0],
            vertices[vertex2 + 1] - vertices[vertex0 + 1],
            vertices[vertex2 + 2] - vertices[vertex0 + 2]    
        );

        // Calculate the normal vector
        glm::vec3 normal = glm::cross(sideVector1, sideVector2);

        // Calculate the normalised version of the normal
        normal = glm::normalize(normal);

        // Find the starting location in the daya array
        // to populate the normal information
        // for each vertex in the given triangular faces
        vertex0 += normalDataOffset;
        vertex1 += normalDataOffset;
        vertex2 += normalDataOffset;
        
        // Add the normal contribution to the 
        // vertex daya array
        vertices[vertex0] += normal.x;
        vertices[vertex0 + 1] += normal.y;
        vertices[vertex0 + 2] += normal.z;

        vertices[vertex1] += normal.x;
        vertices[vertex1 + 1] += normal.y;
        vertices[vertex1 + 2] += normal.z;

        vertices[vertex2] += normal.x;
        vertices[vertex2 + 1] += normal.y;
        vertices[vertex2 + 2] += normal.z;
    }

    for(size_t i = 0; i < numOfVertices/perVertexDataLength; i++)
    {
        // Get the starting location of the normal data in the vertex data array
        unsigned int normalOffset = i * perVertexDataLength + normalDataOffset;

        // Procure the normal vector
        glm::vec3 normal(
            vertices[normalOffset],
            vertices[normalOffset + 1],
            vertices[normalOffset + 2]);
        glm::vec3 normalizedNormal(1.0f, 1.0f, 1.0f);

        // Normalize the normal vector
        normalizedNormal = glm::normalize(normal);

        vertices[normalOffset] = normal.x;
        vertices[normalOffset + 1] = normal.y;
        vertices[normalOffset + 2] = normal.z;
    }
}

void CreateMeshes()
{
    // Specifying the triangular surfaces 
    // using indices (of vertices array) for 
    // index drawing
    unsigned int indices[] = {
        0, 1, 3,
        0, 3, 2,
        3, 1, 2,
        1, 0, 2
    };

    // Vertex positions for the tetrahedron
    GLfloat vertices[] = {
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,    1.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,    0.5f, 1.0f,    0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,    0.5f, 0.0f,    0.0f, 0.0f, 0.0f
    };
    
    // Calculate the averaged normals required
    // for Phong shading that will be later
    // interpolated across all the triangular
    // surfaces of the tetrahedron
    calculateNormalsForPhongShading(
        vertices,
        32,
        indices,
        12,
        8,
        5
    );

    // Generate the tetrahedron meshes
    Mesh *firstTetrahedron = new Mesh();
    firstTetrahedron->createMesh(vertices, indices, 32, 12);
    meshes.push_back(firstTetrahedron);

    Mesh *secondTetrahedron = new Mesh();
    secondTetrahedron->createMesh(vertices, indices, 32, 12);
    meshes.push_back(secondTetrahedron);
}

void CreateShaderPrograms()
{
    // Load the shaders from file and create shader program object
    ShaderManager *shaderManager = new ShaderManager();
    shaderManager->createShaderProgramFromFiles(vertexShaderPath, fragmentShaderPath);
    shaderManagers.push_back(*shaderManager);
}

int main()
{
    // Create a glfw window with an OpenGL context
    window = WindowManager();
    if (!window.createWindow(800, 600, "Phong lighting and shading"))
    {
        printf("Error: Window was not created, exiting the program!\n");
        return 1;
    }

    // Generate the meshes and shaders
    CreateMeshes();
    CreateShaderPrograms();

    // Generate a camera with default
    // parameters to navigate through the scene
    camera = Camera();

    // Load texture
    brickTexture = Texture();
    brickTexture.createTexture("./scenes/simple-texture-mapping/texture-images/brick.png");
    if (!brickTexture.loadTexture())
    {
        printf("Failed to load the brick texture!\n");
        return 1;
    }

    dirtTexture = Texture();
    dirtTexture.createTexture("./scenes/simple-texture-mapping/texture-images/dirt.png");
    if (!dirtTexture.loadTexture())
    {
        printf("Failed to load the dirt texture!\n");
        return 1;
    }

    // Initialising the lights in the scene
    light = Light();
    light.setDirectLightDirection(glm::vec3(1.0f, 2.0f, 3.0f));
    light.setAmbientLightIntensity(0.5f);
    light.setDiffuseLightIntensity(0.0f);

    // Initialise the materials for the objects
    shinyMaterial = Material();
    dullMaterial = Material();

    shinyMaterial.setSpecularIntensity(1.0f);
    shinyMaterial.setShininess(32.0f);
    dullMaterial.setSpecularIntensity(0.5f);
    dullMaterial.setShininess(4.0f);

    // Placeholder object for the camera's view matrix
    glm::mat4 view(1.0f);
    GLfloat previousTimeStamp = 0.0f;

    // Use GLM's perspective function to create the projection matrix
    // Arguments: Fov of the frustum in degrees, aspect ratio, 
        // distance from camera to near plane, distance from camera to the far plane
    glm::mat4 projection = glm::perspective(
                                45.0f,
                                window.getBufferAspectRatio(),
                                0.1f,
                                100.0f);

    // Loop until window is closed, a.k.a rendering loop
    while (!window.isWindowClosed())
    {
        // Generate the delta time for current render loop iteration
        GLfloat currentTimeStamp = glfwGetTime();
        GLfloat deltaTime = currentTimeStamp - previousTimeStamp;
        previousTimeStamp = currentTimeStamp;

        // Get and handle user input events
        glfwPollEvents();

        // Update camera parameters based on user inputs and generate
        // the view matrix.
        camera.updateCameraMotion(window.getKeys(), deltaTime);
        camera.updateCameraOrientation(window.getXChange(), window.getYChange());
        camera.generateViewMatrix(view);

        // Color to be used for clearing the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clear both the color buffer as well as the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate the required shader for drawing
        shaderManagers[0].useShader();

            // Set camera position in the shader
            // for specular lighting calculations
            glUniform3f(
                shaderManagers[0].getUniformCameraPosition(),
                camera.getCameraPosition().x,
                camera.getCameraPosition().y,
                camera.getCameraPosition().z);

            // Generate the transformation matrix
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

            // Adding a light to the scene
            light.useLight(
                shaderManagers[0].getUniformLightColorLocation(),
                shaderManagers[0].getUniformDirectLightDirectionLocation(),
                shaderManagers[0].getUniformAmbientIntensityLocation(),
                shaderManagers[0].getUniformDiffuseIntensityLocation());
            
            // Add specular material properties
            shinyMaterial.useMaterial(
                shaderManagers[0].getUniformSpecularIntensityLocation(),
                shaderManagers[0].getUniformShininessLocation()
            );

            // Bind the matrix data to the uniform variable in the shader
            // Arguments: location, number of matrices, 
                // transpose matrices?, pointer to the matrix/matrices
            glUniformMatrix4fv(
                shaderManagers[0].getUniformModelLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(model));

            glUniformMatrix4fv(
                shaderManagers[0].getUniformViewLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(view));

            glUniformMatrix4fv(
                shaderManagers[0].getUniformProjectionLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(projection));

            // Using the brick texture to render the first tetrahedron
            brickTexture.useTexture();

            // Render the first tetrahedron
            meshes[0]->renderMesh();

            // Setting up the model matrix for the second tetrahedron
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));

            // Setting the new model texture into the shader
            glUniformMatrix4fv(
                shaderManagers[0].getUniformModelLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(model));
            
            dullMaterial.useMaterial(
                shaderManagers[0].getUniformSpecularIntensityLocation(),
                shaderManagers[0].getUniformShininessLocation()
            );

            // Using the dirt texture to render the second tetrahedron
            dirtTexture.useTexture();
          
            // Render the second tetrahedron
            meshes[1]->renderMesh();

        // Deactivating shaders for completeness
        glUseProgram(0);

        // Swap buffers after drawing to update the viewport
        window.swapBuffers();
    }

    return 0;
}
