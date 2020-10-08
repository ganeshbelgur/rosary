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

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture.h"

Texture::Texture() :
    m_textureID(0),
    m_width(0),
    m_height(0),
    m_bitDepth(0),
    m_fileLocation(nullptr)
{
}

void Texture::createTexture(const char *fileLocation)
{
    m_fileLocation = fileLocation;
}

bool Texture::loadTextureRGBOnly()
{   
    // Loading the image texture off the disk using the stb library
    unsigned char *textureData = 
        stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);
    
    // If the image file couldn't be read, report it to the user
    if (!textureData)
    {
        printf("Error: Texture::loadTextureRGBOnly(): Failed to load texture at %s\n", m_fileLocation);
        return false;
    }

    // Creating a texture object
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

        // Setup the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture information
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            m_width,
            m_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            textureData);
        
        // Generate the other mipmap levels
        glGenerateMipmap(GL_TEXTURE_2D);
    
    // Unbind the texture object
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free the loaded image texture
    // as it has been copied to the
    // graphics card's memory
    stbi_image_free(textureData);

    return true;
}

bool Texture::loadTextureWithAlpha()
{   
    // Loading the image texture off the disk using the stb library
    unsigned char *textureData = 
        stbi_load(m_fileLocation, &m_width, &m_height, &m_bitDepth, 0);
    
    // If the image file couldn't be read, report it to the user
    if (!textureData)
    {
        printf("Error: Texture::loadTextureWithAlpha(): Failed to load texture at %s\n", m_fileLocation);
        return false;
    }

    // Creating a texture object
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

        // Setup the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture information
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            m_width,
            m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            textureData);
        
        // Generate the other mipmap levels
        glGenerateMipmap(GL_TEXTURE_2D);
    
    // Unbind the texture object
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free the loaded image texture
    // as it has been copied to the
    // graphics card's memory
    stbi_image_free(textureData);

    return true;
}

void Texture::useTexture()
{
    // The active texture function is not
    // really necessary here as we are binding
    // to the default GL_TEXTURE0 texture unit.
    // However, it is present here to remind
    // the developer to update it if the 
    // texture unit has to be different.
    glActiveTexture(GL_TEXTURE0);
    
    // Bind the texture now to attach it to
    // texture unit 0.
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::clearTexture()
{
    // Remove the texture object from
    // graphics card's memory when
    // not required.
    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
    m_width = 0;
    m_height = 0;
    m_bitDepth = 0;
    m_fileLocation = nullptr;
}

Texture::~Texture()
{
    // Clearing the texture object
    // on the graphics card's memory
    // when the Texture object has
    // been delete.
    clearTexture();
}
