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

#include <glm/gtc/matrix_transform.hpp>

#include "directional-light.h"

DirectionalLight::DirectionalLight() : 
    Light(),
    m_directLightDirection(glm::vec3(0.0f, -1.0f, 0.0f)),
    m_shadowMapWidth(2048),
    m_shadowMapHeight(2048),
    m_directLightShadowMap(new DirectionalLightShadowMap())
{
    m_lightProjectMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    m_directLightShadowMap->setShadowWidth(m_shadowMapWidth);
    m_directLightShadowMap->setShadowHeight(m_shadowMapHeight);
}

void DirectionalLight::setDirectLightDirection(glm::vec3 directLightDirection)
{
    m_directLightDirection = glm::normalize(directLightDirection);
}

void DirectionalLight::useLight(
        GLuint lightColorLocation,
        GLuint ambientLightIntensityLocation,
        GLuint diffuseLightIntensityLocation,
        GLuint directLightDirectionLocation)
{
    Light::useLight(
        lightColorLocation,
        ambientLightIntensityLocation,
        diffuseLightIntensityLocation);

    glUniform3f(
        directLightDirectionLocation,
        m_directLightDirection.x,
        m_directLightDirection.y,
        m_directLightDirection.z);
}

void DirectionalLight::computeShadowMap()
{
    m_directLightShadowMap->init();
}

DirectionalLightShadowMap* DirectionalLight::getShadowMap()
{
    return m_directLightShadowMap;
}

glm::mat4 DirectionalLight::computeProjectionViewLightTransform()
{
    return m_lightProjectMatrix * glm::lookAt(
                                    -m_directLightDirection,
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f));;
}

DirectionalLight::~DirectionalLight()
{
}

