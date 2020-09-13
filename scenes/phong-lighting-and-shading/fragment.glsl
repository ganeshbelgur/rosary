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

// Fragment Shader
// IMPORTANT: Make sure to keep the same name for 
// user defined variables here as in the vertex
// shader or any other intermediate shaders!
#version 330

// The vertex color attribute specified in the 
// vertex shader is used here. It gets interpolated
// automatically across the triangular surfaces
// of the tetrahedron.
in vec4 vertexColor;

// The texture coordinate values help with the
// texture mapping. The texture coordinate
// values assigned at each vertex will be
// automatically interpolated.
in vec2 textureCoordinate;

// Getting the interpolated normal values
in vec3 normal;

// Get the interpolated fragment positions
// in world space coordinates
in vec3 worldSpacePosition;

// Blueprint of the light properties
struct LightProperties
{
    vec3 lightColor;
    vec3 directLightDirection;
    float ambientLightIntensity;
    float diffuseLightIntensity;
};

// Blueprint of the material properties
struct Material
{
    float specularIntensity;
    float shininess;
};

// The sampler2D object is referring to
// the default texture unit GL_TEXTURE0.
uniform sampler2D textureSampler;

// Store all the light properties
// in a uniform object so that they
// can be later populated using tan
// instance of the Light class.
uniform LightProperties light;

// Material properties required to
// set the perform specular lighting
// calculations
uniform Material material;

// We calculate the specular lighting
// with respect to the camera position
uniform vec3 cameraPosition;

out vec4 color;

void main()
{
    // Calculate the ambient light at a point based on the ambient light
    vec4 ambientColor = vec4(light.lightColor, 1.0f) * light.ambientLightIntensity;

    // Calculate the diffuse light at a point based on the directional light
    float directLightFactor = max(dot(normalize(normal), normalize(light.directLightDirection)), 0.0f);
    vec4 diffuseColor = vec4(light.lightColor, 1.0f) * light.diffuseLightIntensity * directLightFactor;

    // We initialise the specular color value to black
    // if no specular contribution is nil for a pixel
    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Calculate the specular light only if direct light has an influence
    if(directLightFactor > 0.0f)
    {
        vec3 FragmentToCamera = normalize(cameraPosition - worldSpacePosition);
        vec3 directLightReflectionDirection = reflect(normalize(light.directLightDirection), normalize(normal));

        // Calculate the contribution of the reflected light along the viewing
        // angle for the given pixel
        float specularFactor = dot(FragmentToCamera, directLightReflectionDirection);
        if(specularFactor > 0.0f)
        {
            // Raise the specular factor to the power of the shininess value
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.lightColor * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    // Compute the final pixel (so called) color based on the texture and light contributions
    color = texture(textureSampler, textureCoordinate) * (ambientColor + diffuseColor + specularColor);
}
