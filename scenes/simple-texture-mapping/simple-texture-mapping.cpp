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

// Scene data
WindowManager window;
std::vector<Mesh*> meshes;
std::vector<ShaderManager> shaderManagers;
Camera camera;
Texture brickTexture;
Texture dirtTexture;

// Path to the shader files relative to Rosary's Makefile
static const char* vertexShaderPath = "./scenes/simple-texture-mapping/vertex.glsl";
static const char* fragmentShaderPath = "./scenes/simple-texture-mapping/fragment.glsl";

void CreateMeshes()
{
    // Specifying the triangular surfaces 
    // using indices (of vertices array) for 
    // index drawing
    unsigned int indices[] = {
        1, 0, 3,
        0, 2, 3,
        2, 1, 3,
        0, 1, 2
    };

    // Vertex positions for the tetrahedron
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f
    };
    
    // Generate the tetrahedron mesh
    Mesh *firstTetrahedron = new Mesh();
    firstTetrahedron->createMesh(vertices, indices, 20, 12);
    meshes.push_back(firstTetrahedron);

    Mesh *secondTetrahedron = new Mesh();
    secondTetrahedron->createMesh(vertices, indices, 20, 12);
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
    if (!window.createWindow(800, 600, "Simple Texture Mapping"))
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
    camera.createCamera();

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

            // Generate the transformation matrix
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

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
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f,1.0f));

            // Setting the new model texture into the shader
            glUniformMatrix4fv(
                shaderManagers[0].getUniformModelLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(model));

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
