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

#include "window-manager.h"

WindowManager::WindowManager() :
    m_lastX(0.0f),
    m_lastY(0.0f),
    m_xChange(0.0f),
    m_yChange(0.0f),
    m_isFirstMouseMovement(true),
    m_window(nullptr),
    m_width(1334),
    m_height(768),
    m_bufferWidth(1334),
    m_bufferHeight(768)
{
    // Set all keys being monitored by
    // the window object to be unpressed
    for (size_t i = 0; i < 1024; ++i)
    {
        m_keys[i] = false;
    }
}

bool WindowManager::createWindow(
    GLint windowWidth,
    GLint windowHeight,
    const std::string windowName)
{
    // Initial GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!\n");
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

    // Enable window I/O related callbacks,
    // enables for keys and mouse I/O in particular
    createCallbacks();

    // Locks the cursor into the app so that user will
    // not loose the cursor location when outside the window.
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    /// Initialise Glew
    GLenum error = glewInit();
    if (error != GLEW_OK)
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

    // Important technique used to make I/O callback
    // work with the specific window of choice using 
    // static member functions. This function allows
    // us to associate the GLFW window with the 
    // corresponding Window object that we have defined.
    glfwSetWindowUserPointer(m_window, this);

    return true;
}

void WindowManager::createCallbacks()
{
    // Call the respective callbacks and handlers
    glfwSetKeyCallback(m_window, handleKeys);
    glfwSetCursorPosCallback(m_window, handleMouse);
}

GLfloat WindowManager::getXChange()
{
    GLfloat xChange = m_xChange;
    m_xChange = 0.0f;
    return xChange;
}

GLfloat WindowManager::getYChange()
{
    GLfloat yChange = m_yChange;
    m_yChange = 0.0f;
    return yChange;
}

void WindowManager::handleKeys(
    GLFWwindow* glfwWindow,
    int key,
    int code,
    int action,
    int mode)
{
    // Getting back the WindowManager object associated with our
    // glfwWindow object. We are able to retrieve it here because
    // we associated the WindowManager object and the glfwWindow object
    // earlier using glfwSetWindowUserPointer(...)
    WindowManager *windowManager = 
        static_cast<WindowManager*>(glfwGetWindowUserPointer(glfwWindow));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            windowManager->m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            windowManager->m_keys[key] = false;
        }
    }
}

void WindowManager::handleMouse(
    GLFWwindow* glfwWindow,
    double xPosition,
    double yPosition)
{
    WindowManager *windowManager =
        static_cast<WindowManager*>(glfwGetWindowUserPointer(glfwWindow));

    if (windowManager->m_isFirstMouseMovement)
    {
        windowManager->m_lastX = xPosition;
        windowManager->m_lastY = yPosition;
        windowManager->m_isFirstMouseMovement = false;
    }

    // Calculate the change in the values of x and y direction
    windowManager->m_xChange = xPosition - windowManager->m_lastX;
    // Notice we are trying to avoid inverted y movement here 
    windowManager->m_yChange = windowManager->m_lastY - yPosition;

    // Store the current xPosition and yPosition
    // for the next iteration
    windowManager->m_lastX = xPosition;
    windowManager->m_lastY = yPosition;
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
