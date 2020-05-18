#version 150

in  vec3 inPosition;
in vec2 inTexCoord;

out vec2 outTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 LookAtMatrix;

void main(void)
{
	gl_Position = projectionMatrix*LookAtMatrix*mdlMatrix*vec4(inPosition, 1.0);
	outTexCoord = inTexCoord;
}
