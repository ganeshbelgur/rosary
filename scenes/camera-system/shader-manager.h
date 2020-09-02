#pragma once

#include <cstdio> // for printf
#include <string.h> // for strlen
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class ShaderManager
{
public:
    ShaderManager();
    void createShaderProgramFromStrings(
        const char* vertexShaderCode,
        const char* fragmentShaderCode);
    void createShaderProgramFromFiles(
        const char* vertexShaderPath,
        const char* fragmentShaderPath);
    GLuint getUniformModelLocation() { return m_uniformModelLocation; }
    GLuint getUniformProjectionLocation() { return m_uniformProjectionLocation; }
    GLuint getUniformViewLocation() { return m_uniformViewLocation; }
    void useShader();
    void clearShader();

    ~ShaderManager();
private:
    void readShaderFile(
        const char* filePath,
        std::string &contents);
    void compileShader(
        const char* vertexShaderCode,
        const char* fragmentShaderCode);
    void AddShader(
        const char* shaderSource,
        GLenum shaderType);

    GLuint m_shaderProgramID, 
        m_uniformModelLocation, 
        m_uniformProjectionLocation,
        m_uniformViewLocation;
};
