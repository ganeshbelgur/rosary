#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    void createCamera(
        glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
        GLfloat startYaw = -90.0f,
        GLfloat startPitch = 0.0f,
        GLfloat moveSpeed = 4.0f,
        GLfloat cursorMoveSpeed = 0.5f);
    void updateCameraMotion(bool* keys, GLfloat deltaTime);
    void updateCameraOrientation(GLfloat xChange, GLfloat yChange);
    void generateViewMatrix(glm::mat4 &viewMatrix);

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