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
    // I/O event callback handlers, we are 
    // using the following static handler 
    // methods to keep the functionality 
    // within the class itself.
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
