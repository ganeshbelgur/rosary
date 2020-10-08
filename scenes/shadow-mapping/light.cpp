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

#include "light.h"

Light::Light() :
    m_ambientLightIntensity(1.0f),
    m_diffuseLightIntensity(1.0f),
    m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
    m_lightProjectMatrix(glm::mat4(1.0f))
{
}

void Light::setLightColor(glm::vec3 lightColor)
{
    m_lightColor = lightColor;
}

void Light::setAmbientLightIntensity(GLfloat ambientLightIntensity)
{
    m_ambientLightIntensity = ambientLightIntensity;
}

void Light::setDiffuseLightIntensity(GLfloat diffuseLightIntensity)
{
    m_diffuseLightIntensity = diffuseLightIntensity;
}

void Light::useLight(
        GLuint lightColorLocation,
        GLuint ambientLightIntensityLocation,
        GLuint diffuseLightIntensityLocation)
{
    glUniform3f(
        lightColorLocation,
        m_lightColor.x,
        m_lightColor.y,
        m_lightColor.z);

    glUniform1f(ambientLightIntensityLocation, m_ambientLightIntensity);
    glUniform1f(diffuseLightIntensityLocation, m_diffuseLightIntensity);
}

Light::~Light()
{
}
