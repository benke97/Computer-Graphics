#version 150

in  vec3 in_Position;
in vec2 inTexCoord;
out vec2 outTexCoord;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 wtvMatrix;

void main(void)
{
	gl_Position = projectionMatrix*wtvMatrix*mdlMatrix*vec4(in_Position, 1.0);
	outTexCoord = inTexCoord;
}