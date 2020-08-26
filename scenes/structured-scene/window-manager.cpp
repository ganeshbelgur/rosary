#include "window-manager.h"

WindowManager::WindowManager() :
    m_window(nullptr),
    m_width(800),
    m_height(600),
    m_bufferWidth(800),
    m_bufferHeight(600)
{
}

bool WindowManager::createWindow(
    GLint windowWidth,
    GLint windowHeight,
    const std::string windowName)
{
    // Initial GLFW
    if(!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return false;
    }

    // Setup GLFW window properties
    // OPENGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Initial a GLFW window with a given size
    m_window = glfwCreateWindow(m_width, m_height, windowName.c_str(), NULL, NULL);
    if (!m_window)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return false;
    }

    //Get framebuffer size
    glfwGetFramebufferSize(m_window, &m_bufferWidth, &m_bufferHeight);

    // Set the OpenGL context for GLew to use
    glfwMakeContextCurrent(m_window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    /// Initialise Glew
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
        printf("Error: GLEW initialisation failed, %s", glewGetErrorString(error));
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    // Enable OpenGL's depth testing feature
    // as we trying to render a 3D object
    glEnable(GL_DEPTH_TEST);

    // Set up viewport size
    glViewport(0, 0, m_bufferWidth, m_bufferHeight);

    return true;
}

GLfloat WindowManager::getBufferAspectRatio()
{
    return (GLfloat)m_bufferWidth/(GLfloat)m_bufferHeight;
}

WindowManager::~WindowManager()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
