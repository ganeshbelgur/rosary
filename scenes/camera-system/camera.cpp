#include "camera.h"
#include <stdio.h>

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
    m_yaw += xChange;
    m_pitch += yChange;

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    m_front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front.y = glm::sin(glm::radians(m_pitch));
    m_front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::generateViewMatrix(glm::mat4 &viewMatrix)
{
    viewMatrix = glm::lookAt(m_position, m_position + m_front, m_worldUp);
}

Camera::~Camera()
{
}