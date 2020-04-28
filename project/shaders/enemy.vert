#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec3 fragNormal;
out vec2 outTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 wtvMatrix;
uniform float intensity;

void main(void)
{
	//mat3 rotation = mat3(mdlMatrix);
	fragNormal = in_Normal;
	vec4 scale = vec4(1.0, 1.0, 1.0, 1.0);
	scale.x *= intensity;
	scale.y *= intensity;
	scale.z *= intensity;
	gl_Position = projectionMatrix*wtvMatrix*mdlMatrix*vec4(in_Position, 2/(1*intensity));
	outTexCoord = inTexCoord;
}
