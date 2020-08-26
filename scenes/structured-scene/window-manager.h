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
    bool isWindowClosed() { return glfwWindowShouldClose(m_window); }
    void swapBuffers() { glfwSwapBuffers(m_window); }

    ~WindowManager();
private:
    GLFWwindow* m_window;

    GLint m_width, m_height;
    GLint m_bufferWidth, m_bufferHeight;
};
