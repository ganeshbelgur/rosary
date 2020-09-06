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

#include "camera.h"

Camera::Camera():
    m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_right(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_yaw(-90.0f),
    m_pitch(0.0f),
    m_moveSpeed(4.0f),
    m_cursorMoveSpeed(0.5f)
{
}

void Camera::createCamera(
        glm::vec3 startPosition,
        glm::vec3 worldUp,
        GLfloat startYaw,
        GLfloat startPitch,
        GLfloat moveSpeed,
        GLfloat cursorMoveSpeed)
{
    m_position = startPosition;
    m_worldUp = worldUp;

    m_yaw = startYaw;
    m_pitch = startPitch;
    m_moveSpeed = moveSpeed;
    m_cursorMoveSpeed = cursorMoveSpeed;

    void updateCameraVectors();
}

void Camera::updateCameraMotion(bool* keys, GLfloat deltaTime)
{
    // Calculate camera speed based on the delta time
    // and update the camera position.
    // We do not need to update the camera vectors when
    // moving in a fixed set of directions.
    GLfloat velocity = m_moveSpeed * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        m_position += m_front * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        m_position -= m_front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        m_position -= m_right * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        m_position += m_right * velocity;
    }
}

void Camera::updateCameraOrientation(GLfloat xChange, GLfloat yChange)
{
    // Update the camera's yaw and pitch
    // based on the extent to which the cursor
    // moves around in the viewport
    m_yaw += xChange;
    m_pitch += yChange;

    // Since our camera is a combination of 
    // FPS and free fly based cameras, 
    // we place restrictions on the maximum
    // and minimum allowed pitch.
    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    // Update the camera vectors
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // We only consider the implications of yaw on pitch
    // and not vice versa.
    m_front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front.y = glm::sin(glm::radians(m_pitch));
    m_front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    // We calculate the rest of the camera's
    // coordinate system.
    m_front = glm::normalize(m_front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::generateViewMatrix(glm::mat4 &viewMatrix)
{
    // Generates the view matrix
    viewMatrix = glm::lookAt(m_position, m_position + m_front, m_worldUp);
}

Camera::~Camera()
{
}
