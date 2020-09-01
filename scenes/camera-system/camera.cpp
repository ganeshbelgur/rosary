#include "camera.h"

Camera::Camera():
    m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_right(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_moveSpeed(0.0f),
    m_cursorMoveSpeed(0.0f)
{
}

bool Camera::createCamera(
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

void Camera::updateCameraVectors()
{
    m_front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front.y = glm::sin(glm::radians(m_pitch));
    m_front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}