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

// Vertex Shader
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTextureCoordinate;
layout (location = 2) in vec3 inNormal;

// Contains the scaling and rotation
// information for the tetrahedron.
uniform mat4 model;

// Contains the projection information
// for the tetrahedron.
uniform mat4 projection;

// Contains the camera orientation
// information to view the tetrahedron.
uniform mat4 view;

uniform mat4 directionalLightTransform;

// Specifying the vertex color attribute for each of the 
// vertices in the vertex shader so that it will be later 
// used in the fragment shader for interpolation
out vec4 vertexColor;

// Just like the vertexColor, the texture coordinates
// assigned to each vertex in the vertex shader will be
// later used in the fragment shader for texture mapping
// after the texture coordinate values get interpolated
out vec2 textureCoordinate;

// Transformed normals from object space to world space
out vec3 normal;

// We need the world space fragment
// position to calculate specular lighting
out vec3 worldSpacePosition;

out vec4 directionalLightSpacePosition;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
    textureCoordinate = inTextureCoordinate;

    // A transformation cannot be directly applied
    // to normals (especially, non-uniform transformations).
    // Instead we make use of the transpose of the inverse of the
    // transformation being applied on the normals.
    normal = mat3(transpose(inverse(model))) * inNormal;

    // Convert the vertex position from local space to
    // world space so that we get access to the corresponding
    // fragment position in the world space
    worldSpacePosition = (model * vec4(position, 1.0f)).xyz;

    directionalLightSpacePosition = directionalLightTransform * model * vec4(position, 1.0f);
}
