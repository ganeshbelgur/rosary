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

#include <glad/glad.h>

#include "constants.h"
#include "directional-light.h"
#include "point-light.h"
#include "spot-light.h"

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

    GLuint getUniformModelLocation();
    GLuint getUniformProjectionLocation();
    GLuint getUniformViewLocation();
    GLuint getUniformSpecularIntensityLocation();
    GLuint getUniformShininessLocation();
    GLuint getUniformCameraPosition();

    void setDirectionalLightData(DirectionalLight *light);
    void setPointLightsData(PointLight *lights, unsigned int lightCount);
    void setSpotLightsData(SpotLight *lights, unsigned int lightCount);
    void setPrimaryTexture(GLuint textureUnit);
    void setDirectionalLightTransform(glm::mat4 lightTransform);
    void setDirectionalLightShadowMap(GLuint textureUnit);

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

    struct LightBaseProperties
    {
        LightBaseProperties();
        void resetInstance();

        GLuint m_uniformLightColorLocation,
            m_uniformAmbientIntensityLocation,
            m_uniformDiffuseIntensityLocation;
    };

    struct DirectLightProperties
    {
        DirectLightProperties();
        void resetInstance();

        LightBaseProperties base;
        GLuint m_uniformDirectLightDirectionLocation;
    } m_uniformDirectionalLight;

    struct PointLightProperties
    {
        PointLightProperties();
        void resetInstance();
 
        LightBaseProperties base;
        GLuint m_uniformPointLightPositionLocation,
            m_uniformConstantLocation,
            m_uniformLinearLocation,
            m_uniformExponentLocation;
    } m_uniformPointLights[MAX_POINT_LIGHTS];

    struct SpotLightProperties
    {
        SpotLightProperties();
        void resetInstance();

        PointLightProperties pointLightBase;
        GLuint m_uniformSpotLightDirectionLocation,
            m_uniformCosineCutOffLocation;
    } m_uniformSpotLights[MAX_SPOT_LIGHTS];

    GLuint m_shaderProgramID, 
        m_uniformModelLocation, 
        m_uniformProjectionLocation,
        m_uniformViewLocation,
        m_uniformSpecularIntensityLocation,
        m_uniformShininessLocation,
        m_uniformCameraPosition,
        m_uniformNumberOfPointLightsLocation,
        m_uniformNumberOfSpotLightsLocation,
        m_uniformPrimaryTextureLocation,
        m_uniformDirectionalLightTransformLocation,
        m_uniformDirectionalLightShadowMapLocation;
};
