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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm::lookAt()

class Camera
{
public:
    Camera();

    // Generates a camera at origin and pointing in the
    // negative z-axis
    void createCamera(
        glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat startYaw = -90.0f,
        GLfloat startPitch = 0.0f,
        GLfloat moveSpeed = 4.0f,
        GLfloat cursorMoveSpeed = 0.5f);

    // Updates camera parameters to reflect the user inputs
    void updateCameraMotion(bool* keys, GLfloat deltaTime);
    void updateCameraOrientation(GLfloat xChange, GLfloat yChange);
    
    // Generates the final view matrix 
    void generateViewMatrix(glm::mat4 &viewMatrix);

    ~Camera();
private:
    // Recalculates all the camera parameters
    void updateCameraVectors();

    // Camera parameters
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;

    // Camera movement parameters
    GLfloat m_yaw;
    GLfloat m_pitch;
    GLfloat m_moveSpeed;
    GLfloat m_cursorMoveSpeed;
};
