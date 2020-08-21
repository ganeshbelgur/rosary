#include <iostream>
#include <cstdio>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Identifiers for the GL objects
GLuint VAO, VBO, shaderProgram;

// Vertex Shader
static const char* vShader = "                                                  \n\
#version 330                                                                    \n\
                                                                                \n\
layout (location = 0) in vec3 position;                                         \n\
                                                                                \n\
void main()                                                                     \n\
{                                                                               \n\
    gl_Position = vec4(0.4 * position.x, 0.4 * position.y, position.z, 1.0f);   \n\
}";

// Fragment Shader
static const char* fShader = "                                      \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 color;                                                     \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);                           \n\
}";

void CreateTriangle()
{
    // Vertex positions for the triangle
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Specify a VAO for our triangle object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        // Specify a VBO to bind to the above VAO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // Loading up the data of the triangle into the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Setting up attribute pointer for shader access
            // Layout location, size, type, normalise?, stride, offset
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            // Layout location
            glEnableVertexAttribArray(0);
        
        // Unbinding the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbinding the VAO
    glBindVertexArray(0);
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

    // Set the OpenGL context for GLEW to use
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

    //Get framebuffer size
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set up viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Generate the triangle and shaders
    CreateTriangle();
    CompileShaderProgram();

    // Loop until window is closed
    while(!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the required shader for drawing
        glUseProgram(shaderProgram);
            // Bind the required object's VAO
            glBindVertexArray(VAO);
                // Perform the draw call to initialise the pipeline
                // drawing mode, offset, number of points
                glDrawArrays(GL_TRIANGLES, 0, 3);
            
            // Unbinding just for completeness
            glBindVertexArray(0);
        // Deactivating shaders for completeness
        glUseProgram(0);

        // Swap buffers after drawing to update the viewport
        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
