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

// Scene data
WindowManager window;
std::vector<Mesh*> meshes;
std::vector<ShaderManager> shaderManagers;

// Path to the shader files relative to Rosary's Makefile
static const char* vertexShaderPath = "./scenes/structured-scene/vertex.glsl";
static const char* fragmentShaderPath = "./scenes/structured-scene/fragment.glsl";

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
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, -1.0f, 1.0f
    };
    
    // Generate the tetrahedron mesh
    Mesh *tetrahedron = new Mesh();
    tetrahedron->createMesh(vertices, indices, 12, 12);
    meshes.push_back(tetrahedron);
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
    if (!window.createWindow(800, 600, "Structured Scene"))
    {
        printf("Error: Window was not created, exiting the program!\n");
        return 1;
    }

    // Generate the meshes and shaders
    CreateMeshes();
    CreateShaderPrograms();

    // Use GLM's perspective function to create the projection matrix
    // Arguments: Fov of the frustum in degrees, aspect ratio, 
        // distance from camera to near plane, distance from camera to the far plane
    glm::mat4 projection = glm::perspective(
                                45.0f,
                                window.getBufferAspectRatio(),
                                0.1f,
                                100.0f);

    // Loop until window is closed, a.k.a rendering loop
    while(!window.isWindowClosed())
    {
        // Get and handle user input events
        glfwPollEvents();

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
                shaderManagers[0].getUniformProjectionLocation(),
                1,
                GL_FALSE,
                glm::value_ptr(projection));

            // Render the tetrahedron
            meshes[0]->renderMesh();

        // Deactivating shaders for completeness
        glUseProgram(0);

        // Swap buffers after drawing to update the viewport
        window.swapBuffers();
    }

    return 0;
}
