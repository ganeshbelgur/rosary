#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Identifiers for the GL objects
GLuint VAO, VBO, IBO, shaderProgram, uniformModel;

// Parameters to rotate the tetrahedron
const float toRadians = 3.14159265f / 180.0f;
float curveAngle = 0.0f;

// Vertex Shader
// Specifying the vertex color attribute for each of the 
// vertices in the vertex shader that will be later used
// in the fragment shader for interpolation
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 position;                     \n\
                                                            \n\
out vec4 vertexColor;                                       \n\
                                                            \n\
// Contains the scaling and rotation                        \n\
// information for the tetrahedron.                         \n\
uniform mat4 model;                                         \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
    gl_Position = model * vec4(position, 1.0f);             \n\
    vertexColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f);  \n\
}";

// Fragment Shader
// The vertex color attribute in used in the fragment shader
// and it gets interpolated automatically across the 
// triangular surfaces of the tetrahedron.
static const char* fShader = "      \n\
#version 330                        \n\
                                    \n\
in vec4 vertexColor;                \n\
                                    \n\
out vec4 color;                     \n\
                                    \n\
void main()                         \n\
{                                   \n\
    color = vertexColor;            \n\
}";

void CreateTetrahedron()
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

    // Specify a VAO for our tetrahedron object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        // Specify a VBO to pass in the index data
        // which will be used for index drawing
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Specify a VBO to bind to the above VAO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // Loading up the data of the tetrahedron into the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Setting up attribute pointer for shader access
            // Layout location, size, type, normalise?, stride, offset
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            // Layout location
            glEnableVertexAttribArray(0);

    // Unbinding the VAO
    glBindVertexArray(0);
        // Unbinding the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Unbinding the IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint program, const char* shaderSource, GLenum shaderType)
{
    // Create an empty shader object
    GLuint shader = glCreateShader(shaderType);

    // Convert the source code for the shader 
    // into the right type for loading
    const GLchar* sourceCode[1];
    sourceCode[0] = shaderSource;

    // Convert the length of the source code for 
    // the shader into the right type for loading
    GLint sourceLength[1];
    sourceLength[0] = strlen(shaderSource);

    // Load the shader source into the 
    // empty shader object and compile the code
    glShaderSource(shader, 1, sourceCode, sourceLength);
    glCompileShader(shader);

    // Find and log error if any from 
    // the above compilation process
    GLint result = 0;
    GLchar log[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("Error: Compilation of shader of type %d failed, '%s'\n", shaderType, log);
        return;
    }

    // Attach the shader to the shader program
    glAttachShader(program, shader);
}

void CompileShaderProgram()
{
    // Create an empty shader program object
    shaderProgram = glCreateProgram();

    // Check if it was created successfully
    if(!shaderProgram)
    {
        printf("Error: Generation of shader program failed!\n");
        return;
    }

    // Attaching our vertex and fragment shaders to the shader program
    AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    // Setting up error logging objects
    GLint result = 0;
    GLchar log[1024] = { 0 };

    // Perform shader program linking
    glLinkProgram(shaderProgram);

    // Find and log errors if any from the linking process done above
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
        printf("Error: Linking of the shader program failed, '%s'\n", log);
        return;
    }

    // Perform shader program validation
    glValidateProgram(shaderProgram);

    // Find and log errors if any from the validation process done above
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
        printf("Error: Shader program validation failed, '%s'", log);
        return;
    }

    // Get the location of the uniform variable to
    // provide the transform information to the shader
    uniformModel = glGetUniformLocation(shaderProgram, "model");
}

int main()
{
    // Initial GLFW
    if(!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OPENGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Initial a GLFW window with a given size
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLApp", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Set the OpenGL context for GLew to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    /// Initialise Glew
    if(glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Enable OpenGL's depth testing feature
    // as we trying to render a 3D object
    glEnable(GL_DEPTH_TEST);

    //Get framebuffer size
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set up viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Generate the tetrahedron and shaders
    CreateTetrahedron();
    CompileShaderProgram();

    // Loop until window is closed, a.k.a rendering loop
    while(!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        // Handling rotation parameters
        curveAngle += 0.8f;
        if(curveAngle > 360)
        {
            curveAngle -= 360.0f;
        }

        // Color to be used for clearing the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // Clear both the color buffer as well as the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate the required shader for drawing
        glUseProgram(shaderProgram);

            // Generate the transformation matrix
            glm::mat4 model(1.0f);
            model = glm::rotate(model, curveAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            // Bind the matrix data to the uniform variable in the shader
            // Arguments: location, number of matrices, transpose matrices?, pointer to the matrix/matrices
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

            // Bind the required object's VAO
            glBindVertexArray(VAO);
                // Bind the IBO when using glDrawElements
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				    // Drawing with glDrawElements instead
                        // of glDrawArrays as we using an IBO.
                    // Perform the draw call to initialise the pipeline.
                    // Arguments: drawing mode, number of indices, type of the index data, 
                        // pointer to indices (0 because already passed)
                    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);            
            // Unbinding just for completeness
            glBindVertexArray(0);
                // Unbinding the IBO
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Deactivating shaders for completeness
        glUseProgram(0);

        // Swap buffers after drawing to update the viewport
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}