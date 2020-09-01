// Fragment Shader
// The vertex color attribute in used in the fragment shader
// and it gets interpolated automatically across the 
// triangular surfaces of the tetrahedron.
#version 330

in vec4 vertexColor;

out vec4 color;

void main()
{
    color = vertexColor;
}
