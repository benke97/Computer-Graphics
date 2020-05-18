#version 330 core

// Interpolated values from the vertex shaders
in vec4 particleColor;
in vec2 UV;

// Ouput data
out vec4 color;

uniform sampler2D particleTex;
void main(void)
{
	// Output color = color of the texture at the specified UV
	color = texture( particleTex, UV ) * particleColor;
}
