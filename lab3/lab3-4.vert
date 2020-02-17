#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
out vec3 inNormal;
in vec2 inTexCoord;
out vec2 outTexCoord;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 wtvMatrix;
uniform vec3 lightSourcesDirPosArr[4];
out vec3 out_Position;
void main(void)
{
	out_Position = vec3(mdlMatrix*vec4(in_Position,1.0));
	mat3 rotation = mat3(mdlMatrix);
	inNormal = rotation*in_Normal;
	gl_Position = projectionMatrix*wtvMatrix*mdlMatrix*vec4(in_Position, 1.0);
	outTexCoord = inTexCoord;
}
