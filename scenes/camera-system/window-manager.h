#pragma once

#include <iostream>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
    WindowManager();
    bool createWindow(
        GLint windowWidth,
        GLint windowHeight,
        const std::string windowName);
    GLint getBufferWidth() { return m_bufferWidth; }
    GLint getBufferheight() { return m_bufferHeight; }
    GLfloat getBufferAspectRatio();

    // Some hacky convenience function for handling
    // camera movements
    bool* getKeys() { return m_keys; }
    GLfloat getXChange();
    GLfloat getYChange();

    bool isWindowClosed() { return glfwWindowShouldClose(m_window); }
    void swapBuffers() { glfwSwapBuffers(m_window); }

    ~WindowManager();
private:
    // Involes keyboard and mouse input 
    // callbacks and handlers
    void createCallbacks();

    // To avoid having global functions as
    // callback handlers, we are using the 
    // following static handler methods to
    // keep the functionality within the 
    // class itself.
    static void handleKeys(
        GLFWwindow* glfwWindow,
        int key,
        int code,
        int action,
        int mode);
    static void handleMouse(
        GLFWwindow* glfwWindow,
        double xPosition,
        double yPosition);

    // Keyboard and mouse input data
    GLfloat m_lastX;
    GLfloat m_lastY;
    GLfloat m_xChange;
    GLfloat m_yChange;
    bool m_isFirstMouseMovement;
    bool m_keys[1024];

    // Window data
    GLFWwindow* m_window;
    GLint m_width, m_height;
    GLint m_bufferWidth, m_bufferHeight;
};
