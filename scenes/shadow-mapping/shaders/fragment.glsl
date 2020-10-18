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

in vec4 directionalLightSpacePosition;

// We keep this in sync with the constant
// values specified in the constants.h file
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

// Blueprint of the base light properties
struct LightBaseProperties
{
    vec3 lightColor;
    float ambientLightIntensity;
    float diffuseLightIntensity;
};

// Blueprint of the direct light properties
struct DirectLightProperties
{
    LightBaseProperties base;
    vec3 directLightDirection;
};

// Blueprint of the point light properties
struct PointLightProperties
{
    LightBaseProperties base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

// Blueprint of the spot light properties
struct SpotLightProperties
{
    PointLightProperties pointLightBase;
    vec3 direction;
    float cosineCutOffAngle;
};

// The number of active point lights
// out of the available MAX_POINT_LIGHTS
uniform int numberOfPointLights;

// The number of active spot lights
// out of the available MAX_SPOT_LIGHTS
uniform int numberOfSpotLights;

// Store all the light properties
// in a uniform object so that they
// can be later populated using an
// instance of the Light class.
uniform DirectLightProperties directLight;

// Initialise the list of point lights
uniform PointLightProperties pointLights[MAX_POINT_LIGHTS];

// Initialise the list of spot lights
uniform SpotLightProperties spotLights[MAX_SPOT_LIGHTS];

// Blueprint of the material properties
struct Material
{
    float specularIntensity;
    float shininess;
};

// The sampler2D object is referring to
// the default texture unit GL_TEXTURE0.
uniform sampler2D textureSampler;
uniform sampler2D directionalLightShadowMapSampler;

// Material properties required to
// set the perform specular lighting
// calculations
uniform Material material;

// We calculate the specular lighting
// with respect to the camera position
uniform vec3 cameraPosition;

out vec4 color;


float calculateDirectionalLightShadowFactor(DirectLightProperties light)
{
    // Converting our position to normalised device coordinates
    vec3 projectionCoordinates = directionalLightSpacePosition.xyz / directionalLightSpacePosition.w;
    
    // Mapping our coordinates between 0.0f and 1.0f
    projectionCoordinates = (projectionCoordinates * 0.5) + 0.5;
        
    float currentDepth = projectionCoordinates.z;

    vec3 newNormal = normalize(normal);
    vec3 lightDirection = normalize(light.directLightDirection);

    float bias = max(0.05 * (1 - dot(newNormal, lightDirection)), 0.005);
    float shadowFactor = 0.0; 
    vec2 texelSize = 1.0 / textureSize(directionalLightShadowMapSampler, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directionalLightShadowMapSampler, projectionCoordinates.xy + vec2(x, y) * texelSize).r;
            shadowFactor += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadowFactor /= 9.0;

    if (projectionCoordinates.z > 1.0)
    {
        shadowFactor = 0.0;
    }
    return shadowFactor;
}

vec4 calculateLightContribution(LightBaseProperties light, vec3 direction, float shadowFactor)
{
    // Calculate the ambient light at a point based on the ambient light
    vec4 ambientColor = vec4(light.lightColor, 1.0f) * light.ambientLightIntensity;

    // Calculate the diffuse light at a point based on the direction of the arriving light
    float diffuseLightFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.lightColor, 1.0f) * light.diffuseLightIntensity * diffuseLightFactor;

    // We initialise the specular color value to black
    // if no specular contribution is nil for a pixel
    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Calculate the specular light only if diffuse lighting occurs
    if (diffuseLightFactor > 0.0f)
    {
        vec3 FragmentToCamera = normalize(cameraPosition - worldSpacePosition);
        vec3 lightReflectionDirection = normalize(reflect(normalize(direction), normalize(normal)));

        // Calculate the contribution of the reflected light along the viewing
        // angle for the given pixel
        float specularFactor = dot(FragmentToCamera, lightReflectionDirection);
        if (specularFactor > 0.0f)
        {
            // Raise the specular factor to the power of the shininess value
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.lightColor * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 calculateDirectLight()
{
    float shadowFactor = calculateDirectionalLightShadowFactor(directLight);
    return calculateLightContribution(directLight.base, directLight.directLightDirection, shadowFactor);
}

vec4 calculatePointLight(PointLightProperties pointLight)
{
    // Calculate the direction and distance to the fragment
    // position from the point light position
    vec3 direction = worldSpacePosition - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    // Calculate the lighting contributions
    vec4 colorContribution = calculateLightContribution(pointLight.base, direction, 0.0f);

    // Calculating the attenuation coefficient
    float attenuation = pointLight.exponent * distance * distance +
                        pointLight.linear * distance +
                        pointLight.constant;
        
    // Add the total color contributions from the given point light
    return (colorContribution / attenuation);
}

vec4 calculatePointLights()
{
    // A variable to store the contributions of all point lights
    // at a fragment position
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Loop through all the point lights and find their contributions
    for (int i = 0; i < numberOfPointLights; ++i)
    {   
        // Add the total color contributions from the given point light
        totalColor += calculatePointLight(pointLights[i]);
    }

    return totalColor;
}

vec4 calculateSpotLight(SpotLightProperties spotLight)
{
    // Calculate the cosine of the angle between the spot light direction
    // and the direction to the fragment from the spot light position
    vec3 direction = normalize(worldSpacePosition - spotLight.pointLightBase.position);
    float cosineAngle = dot(spotLight.direction, direction);

    // If the fragment position is lit, we reuse point calculation steps
    if (spotLight.cosineCutOffAngle < cosineAngle)
    {
        vec4 colorContribution = calculatePointLight(spotLight.pointLightBase);

        // Provides feathering effect to make the edges of the spot light appear smoother
        return colorContribution * (1.0f - (1.0f - cosineAngle) * (1.0f / (1.0f - spotLight.cosineCutOffAngle)));
    }
    else
    {
        // If the fragment position is not lit, we return no color contribution
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

vec4 calculateSpotLights()
{
    // A variable to store the contributions of all spot lights
    // at a fragment position
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < numberOfSpotLights; ++i)
    {
        // Add the total color contributions from the given spot light
        totalColor += calculateSpotLight(spotLights[i]);
    }

    return totalColor;
}

void main()
{
    // Calculate the contribution of light intensity
    // color values arriving at a fragment position
    // by considering a directional light and all 
    // available point lights and spot lights
    vec4 lightingColor = calculateDirectLight();
    lightingColor += calculatePointLights();
    lightingColor += calculateSpotLights();

    // Compute the final pixel (so called) color based on the texture and light contributions
    color = texture(textureSampler, textureCoordinate) * lightingColor;
}
