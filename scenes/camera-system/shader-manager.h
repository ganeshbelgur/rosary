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
