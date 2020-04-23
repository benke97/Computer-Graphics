#version 330

// Interpolated values from the vertex shaders
in vec4 particleColor;

// Ouput data
out vec4 color;


void main(void)
{
	// Output color = color of the texture at the specified UV
  color = particleColor;

}
