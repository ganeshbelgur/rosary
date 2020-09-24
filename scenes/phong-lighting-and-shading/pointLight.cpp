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

#include "pointLight.h"

PointLight::PointLight() :
    Light(),
    m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_constant(1.0f),
    m_linear(0.0f),
    m_exponent(0.0f)
{
}

void PointLight::setPosition(glm::vec3 position)
{
    m_position = position;
}

void PointLight::setConstantAttenuationComponent(GLfloat constant)
{
    m_constant = constant;
}

void PointLight::setLinearAttenuationComponent(GLfloat linear)
{
    m_linear = linear;
}

void PointLight::setExponentAttenuationComponent(GLfloat exponent)
{
    m_exponent = exponent;
}

void PointLight::useLight(GLuint lightColorLocation,
        GLuint ambientLightIntensityLocation,
        GLuint diffuseLightIntensityLocation,
        GLuint positionLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint exponentLocation)
{
    Light::useLight(
        lightColorLocation,
        ambientLightIntensityLocation,
        diffuseLightIntensityLocation);

    glUniform3f(
        positionLocation,
        m_position.x,
        m_position.y,
        m_position.z);
    
    glUniform1f(
        constantLocation,
        m_constant
    );

    glUniform1f(
        linearLocation,
        m_linear);
    
    glUniform1f(
        exponentLocation,
        m_exponent);
}

PointLight::~PointLight()
{
}
