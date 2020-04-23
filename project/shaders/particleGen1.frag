#version 330

// Interpolated values from the vertex shaders
//in vec2 UV;
in vec4 particleColor;

// Ouput data
out vec4 color;

//uniform sampler2D myTextureSampler;

void main(void)
{
	// Output color = color of the texture at the specified UV
	//color = texture( myTextureSampler, UV ) * particleColor;
  color = particleColor;

}
