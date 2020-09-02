// Vertex Shader
// Specifying the vertex color attribute for each of the 
// vertices in the vertex shader that will be later used
// in the fragment shader for interpolation
#version 330

layout (location = 0) in vec3 position;

out vec4 vertexColor;

// Contains the scaling and rotation
// information for the tetrahedron.
uniform mat4 model;

// Contains the projection information
// for the tetrahedron.
uniform mat4 projection;

// Contains the camera orientation
// information to view the tetrahedron.
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
}
