#version 150

in vec3 in_Position;
in vec3 in_Normal;

out vec3 fragNormal;
out vec2 outTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 wtvMatrix;
uniform float intensity;

void main(void)
{
	fragNormal = in_Normal;
	gl_Position = projectionMatrix*wtvMatrix*mdlMatrix*vec4(in_Position, 2/(1*intensity));
	outTexCoord = inTexCoord;
}
