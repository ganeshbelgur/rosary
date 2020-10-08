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

#include "directional-light-shadow-map.h"

DirectionalLightShadowMap::DirectionalLightShadowMap() :
    m_FBO(0),
    m_shadowMap(0),
    m_shadowWidth(4096),
    m_shadowHeight(4096)
{
}

void DirectionalLightShadowMap::setShadowWidth(GLuint width)
{
    m_shadowWidth = width;
}

void DirectionalLightShadowMap::setShadowHeight(GLuint height)
{
    m_shadowHeight = height;
}

GLuint DirectionalLightShadowMap::getShadowWidth()
{
    return m_shadowWidth;
}

GLuint DirectionalLightShadowMap::getShadowHeight()
{
    return m_shadowHeight;
}

bool DirectionalLightShadowMap::init()
{
    // Generate a new frame buffer object
    glGenFramebuffers(1, &m_FBO);

    // Generate a texture object for the shadow map
    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);

    // Passing nullptr because we do not have any 
    // texture data to pass through but we still 
    // want the texture to be initialised with 0.0f
    // for every pixel's depth component
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        m_shadowWidth,
        m_shadowHeight,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr);

    // Setup the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // Attach the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // Check the status of the frame buffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Error: Framebuffer status is %i\n", status);
        return false;
    }

    // Unbind the framebuffer so we set the
    // binding back to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void DirectionalLightShadowMap::write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void DirectionalLightShadowMap::read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

DirectionalLightShadowMap::~DirectionalLightShadowMap()
{
    if (m_FBO)
    {
        glDeleteFramebuffers(1, &m_FBO);
        m_FBO = 0;
    }

    if (m_shadowMap)
    {
        glDeleteTextures(1, &m_shadowMap);
        m_shadowMap = 0;
    }
}
