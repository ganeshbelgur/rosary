#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    bool createCamera(
        glm::vec3 startPosition,
        glm::vec3 worldUp,
        GLfloat startYaw,
        GLfloat startPitch,
        GLfloat moveSpeed,
        GLfloat cursorMoveSpeed);

    ~Camera();
private:
    void updateCameraVectors();

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;

    GLfloat m_yaw;
    GLfloat m_pitch;
    GLfloat m_moveSpeed;
    GLfloat m_cursorMoveSpeed;
};