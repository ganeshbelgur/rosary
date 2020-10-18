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

#include <glm/gtc/type_ptr.hpp>

#include "shader-manager.h"

ShaderManager::ShaderManager():
    m_uniformDirectionalLight(DirectLightProperties()),
    m_shaderProgramID(0),
    m_uniformModelLocation(0),
    m_uniformProjectionLocation(0),
    m_uniformViewLocation(0),
    m_uniformSpecularIntensityLocation(0),
    m_uniformShininessLocation(0),
    m_uniformCameraPosition(0),
    m_uniformNumberOfPointLightsLocation(0),
    m_uniformNumberOfSpotLightsLocation(0),
    m_uniformPrimaryTextureLocation(0),
    m_uniformDirectionalLightTransformLocation(0),
    m_uniformDirectionalLightShadowMapLocation(0)
{
    for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        m_uniformPointLights[i] = PointLightProperties();
    }

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i)
    {
        m_uniformSpotLights[i] = SpotLightProperties();
    }
}

void ShaderManager::createShaderProgramFromStrings(
        const char* vertexShaderCode,
        const char* fragmentShaderCode)
{
    compileShader(vertexShaderCode, fragmentShaderCode);
}

void ShaderManager::createShaderProgramFromFiles(
    const char* vertexShaderPath,
    const char* fragmentShaderPath)
{
    std::string vertexShaderString, fragmentShaderString;
    readShaderFile(vertexShaderPath, vertexShaderString);
    readShaderFile(fragmentShaderPath, fragmentShaderString);
    compileShader(vertexShaderString.c_str(), fragmentShaderString.c_str());
}

GLuint ShaderManager::getUniformModelLocation()
{
    return m_uniformModelLocation;
}

GLuint ShaderManager::getUniformProjectionLocation()
{
    return m_uniformProjectionLocation;
}

GLuint ShaderManager::getUniformViewLocation()
{
    return m_uniformViewLocation;
}

GLuint ShaderManager::getUniformSpecularIntensityLocation()
{
    return m_uniformSpecularIntensityLocation;
}

GLuint ShaderManager::getUniformShininessLocation()
{
    return m_uniformShininessLocation;
}

GLuint ShaderManager::getUniformCameraPosition()
{
    return m_uniformCameraPosition;
}

void ShaderManager::setDirectionalLightData(DirectionalLight *light)
{
    light->useLight(
        m_uniformDirectionalLight.base.m_uniformLightColorLocation,
        m_uniformDirectionalLight.base.m_uniformAmbientIntensityLocation,
        m_uniformDirectionalLight.base.m_uniformDiffuseIntensityLocation,
        m_uniformDirectionalLight.m_uniformDirectLightDirectionLocation);
}

void ShaderManager::setPointLightsData(PointLight *lights, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
    glUniform1i(m_uniformNumberOfPointLightsLocation, lightCount);

    for (size_t i = 0; i < lightCount; ++i)
    {
        lights[i].useLight(
            m_uniformPointLights[i].base.m_uniformLightColorLocation,
            m_uniformPointLights[i].base.m_uniformAmbientIntensityLocation,
            m_uniformPointLights[i].base.m_uniformDiffuseIntensityLocation,
            m_uniformPointLights[i].m_uniformPointLightPositionLocation,
            m_uniformPointLights[i].m_uniformConstantLocation,
            m_uniformPointLights[i].m_uniformLinearLocation,
            m_uniformPointLights[i].m_uniformExponentLocation);
    }
}

void ShaderManager::setSpotLightsData(SpotLight *lights, unsigned int lightCount)
{
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
    glUniform1i(m_uniformNumberOfSpotLightsLocation, lightCount);

    for (size_t i = 0; i < lightCount; ++i)
    {
        lights[i].useLight(
            m_uniformSpotLights[i].pointLightBase.base.m_uniformLightColorLocation,
            m_uniformSpotLights[i].pointLightBase.base.m_uniformAmbientIntensityLocation,
            m_uniformSpotLights[i].pointLightBase.base.m_uniformDiffuseIntensityLocation,
            m_uniformSpotLights[i].pointLightBase.m_uniformPointLightPositionLocation,
            m_uniformSpotLights[i].pointLightBase.m_uniformConstantLocation,
            m_uniformSpotLights[i].pointLightBase.m_uniformLinearLocation,
            m_uniformSpotLights[i].pointLightBase.m_uniformExponentLocation,
            m_uniformSpotLights[i].m_uniformSpotLightDirectionLocation,
            m_uniformSpotLights[i].m_uniformCosineCutOffLocation);
    }
}

void ShaderManager::setPrimaryTexture(GLuint textureUnit)
{
    glUniform1i(m_uniformPrimaryTextureLocation, textureUnit);
}

void ShaderManager::setDirectionalLightTransform(glm::mat4 lightTransform)
{
    glUniformMatrix4fv(
        m_uniformDirectionalLightTransformLocation,
        1,
        GL_FALSE,
        glm::value_ptr(lightTransform));
}

void ShaderManager::setDirectionalLightShadowMap(GLuint textureUnit)
{
    glUniform1i(m_uniformDirectionalLightShadowMapLocation, textureUnit);
}

void ShaderManager::readShaderFile(const char* filePath, std::string &contents)
{
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open())
    {
        printf(
            "Error: Failed to read file at %s , file does not exist.",
            filePath);
        return;
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        contents.append(line + "\n");
    }

    fileStream.close();
}

void ShaderManager::compileShader(
        const char* vertexShaderCode,
        const char* fragmentShaderCode)
{
    // Create an empty shader program object
    m_shaderProgramID = glCreateProgram();

    // Check if it was created successfully
    if (!m_shaderProgramID)
    {
        printf("Error: Generation of shader program failed!\n");
        return;
    }

    // Attaching our vertex and fragment shaders to the shader program
    AddShader(vertexShaderCode, GL_VERTEX_SHADER);
    AddShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

    // Setting up error logging objects
    GLint result = 0;
    GLchar log[1024] = { 0 };

    // Perform shader program linking
    glLinkProgram(m_shaderProgramID);

    // Find and log errors if any from the linking process done above
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_shaderProgramID, sizeof(log), NULL, log);
        printf("Error: Linking of the shader program failed, '%s'\n", log);
        return;
    }

    // Perform shader program validation
    glValidateProgram(m_shaderProgramID);

    // Find and log errors if any from the validation process done above
    glGetProgramiv(m_shaderProgramID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(m_shaderProgramID, sizeof(log), NULL, log);
        printf("Error: Shader program validation failed, '%s'", log);
        return;
    }

    // Get the location of the uniform variables to
    // provide the transform information to the shader
    m_uniformModelLocation = glGetUniformLocation(m_shaderProgramID, "model");
    m_uniformProjectionLocation = glGetUniformLocation(m_shaderProgramID, "projection");
    m_uniformViewLocation = glGetUniformLocation(m_shaderProgramID, "view");
    m_uniformSpecularIntensityLocation = glGetUniformLocation(m_shaderProgramID, "material.specularIntensity");
    m_uniformShininessLocation = glGetUniformLocation(m_shaderProgramID, "material.shininess");
    m_uniformCameraPosition = glGetUniformLocation(m_shaderProgramID, "cameraPosition");
    m_uniformDirectionalLight.base.m_uniformLightColorLocation = glGetUniformLocation(m_shaderProgramID, "directLight.base.lightColor");
    m_uniformDirectionalLight.base.m_uniformAmbientIntensityLocation = glGetUniformLocation(m_shaderProgramID, "directLight.base.ambientLightIntensity");
    m_uniformDirectionalLight.base.m_uniformDiffuseIntensityLocation = glGetUniformLocation(m_shaderProgramID, "directLight.base.diffuseLightIntensity");
    m_uniformDirectionalLight.m_uniformDirectLightDirectionLocation = glGetUniformLocation(m_shaderProgramID, "directLight.directLightDirection");

    // Get point light properties locations
    m_uniformNumberOfPointLightsLocation = glGetUniformLocation(m_shaderProgramID, "numberOfPointLights");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        char locationStringBuffer[100] = {'\0'};
        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].base.lightColor", i);
        m_uniformPointLights[i].base.m_uniformLightColorLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].base.ambientLightIntensity", i);
        m_uniformPointLights[i].base.m_uniformAmbientIntensityLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].base.diffuseLightIntensity", i);
        m_uniformPointLights[i].base.m_uniformDiffuseIntensityLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].position", i);
        m_uniformPointLights[i].m_uniformPointLightPositionLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].constant", i);
        m_uniformPointLights[i].m_uniformConstantLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].linear", i);
        m_uniformPointLights[i].m_uniformLinearLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "pointLights[%lu].exponent", i);
        m_uniformPointLights[i].m_uniformExponentLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);
    }

    // Get spot light properties locations
    m_uniformNumberOfSpotLightsLocation = glGetUniformLocation(m_shaderProgramID, "numberOfSpotLights");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i)
    {
        char locationStringBuffer[100] = {'\0'};
        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.base.lightColor", i);
        m_uniformSpotLights[i].pointLightBase.base.m_uniformLightColorLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.base.ambientLightIntensity", i);
        m_uniformSpotLights[i].pointLightBase.base.m_uniformAmbientIntensityLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.base.diffuseLightIntensity", i);
        m_uniformSpotLights[i].pointLightBase.base.m_uniformDiffuseIntensityLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.position", i);
        m_uniformSpotLights[i].pointLightBase.m_uniformPointLightPositionLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.constant", i);
        m_uniformSpotLights[i].pointLightBase.m_uniformConstantLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.linear", i);
        m_uniformSpotLights[i].pointLightBase.m_uniformLinearLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].pointLightBase.exponent", i);
        m_uniformSpotLights[i].pointLightBase.m_uniformExponentLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].direction", i);
        m_uniformSpotLights[i].m_uniformSpotLightDirectionLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);

        snprintf(locationStringBuffer, sizeof(locationStringBuffer), "spotLights[%lu].cosineCutOffAngle", i);
        m_uniformSpotLights[i].m_uniformCosineCutOffLocation = glGetUniformLocation(m_shaderProgramID, locationStringBuffer);
    }

    m_uniformPrimaryTextureLocation = glGetUniformLocation(m_shaderProgramID, "textureSampler");
    m_uniformDirectionalLightTransformLocation = glGetUniformLocation(m_shaderProgramID, "directionalLightTransform");
    m_uniformDirectionalLightShadowMapLocation = glGetUniformLocation(m_shaderProgramID, "directionalLightShadowMapSampler");
}

void ShaderManager::AddShader(
    const char* shaderSource,
    GLenum shaderType)
{
    // Create an empty shader object
    GLuint shaderID = glCreateShader(shaderType);

    // Convert the source code for the shader 
    // into the right type for loading
    const GLchar* sourceCode[1];
    sourceCode[0] = shaderSource;

    // Convert the length of the source code for 
    // the shader into the right type for loading
    GLint sourceLength[1];
    sourceLength[0] = strlen(shaderSource);

    // Load the shader source into the 
    // empty shader object and compile the code
    glShaderSource(shaderID, 1, sourceCode, sourceLength);
    glCompileShader(shaderID);

    // Find and log error if any from 
    // the above compilation process
    GLint result = 0;
    GLchar log[1024] = { 0 };

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
        printf("Error: Compilation of shader of type %d failed, '%s'\n", shaderType, log);
        return;
    }

    // Attach the shader to the shader program
    glAttachShader(m_shaderProgramID, shaderID);
}

void ShaderManager::useShader()
{
    glUseProgram(m_shaderProgramID);
}

void ShaderManager::clearShader()
{
    if (m_shaderProgramID)
    {
        // Free the shader program from memory (graphics)
        glDeleteProgram(m_shaderProgramID);
        m_shaderProgramID = 0;
    }

    m_uniformModelLocation = 0;
    m_uniformProjectionLocation = 0;
    m_uniformViewLocation = 0;
    m_uniformDirectionalLight.resetInstance();
    m_uniformNumberOfPointLightsLocation = 0;
    m_uniformPrimaryTextureLocation = 0;
    m_uniformDirectionalLightTransformLocation = 0;
    m_uniformDirectionalLightShadowMapLocation = 0;

    for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        m_uniformPointLights[i].resetInstance();
    }

    m_uniformNumberOfSpotLightsLocation = 0;
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i)
    {
        m_uniformSpotLights[i].resetInstance();
    }
}

ShaderManager::~ShaderManager()
{
    // Ensuring the shader is off the graphics card's 
    // memory before deleting the shader manager object
    clearShader();
}

ShaderManager::LightBaseProperties::LightBaseProperties() :
    m_uniformLightColorLocation(0),
    m_uniformAmbientIntensityLocation(0),
    m_uniformDiffuseIntensityLocation(0)
{
}

void ShaderManager::LightBaseProperties::resetInstance()
{
    m_uniformLightColorLocation = 0;
    m_uniformAmbientIntensityLocation = 0;
    m_uniformDiffuseIntensityLocation = 0;
}

ShaderManager::DirectLightProperties::DirectLightProperties() :
    base(LightBaseProperties()),
    m_uniformDirectLightDirectionLocation(0)
{
}

void ShaderManager::DirectLightProperties::resetInstance()
{
    base.resetInstance();
    m_uniformDirectLightDirectionLocation = 0;
}

ShaderManager::PointLightProperties::PointLightProperties() :
    base(LightBaseProperties()),
    m_uniformPointLightPositionLocation(0),
    m_uniformConstantLocation(0),
    m_uniformLinearLocation(0),
    m_uniformExponentLocation(0)
{
}

void ShaderManager::PointLightProperties::resetInstance()
{
    base.resetInstance();
    m_uniformPointLightPositionLocation = 0;
    m_uniformConstantLocation = 0;
    m_uniformLinearLocation = 0;
    m_uniformExponentLocation = 0;
}

ShaderManager::SpotLightProperties::SpotLightProperties() :
    pointLightBase(PointLightProperties()),
    m_uniformSpotLightDirectionLocation(0),
    m_uniformCosineCutOffLocation(0)
{
}

void ShaderManager::SpotLightProperties::resetInstance()
{
    pointLightBase.resetInstance();
    m_uniformSpotLightDirectionLocation = 0;
    m_uniformCosineCutOffLocation = 0;
}
