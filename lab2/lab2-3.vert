#version 150

in  vec3 in_Position;
in  vec3 in_Normal;
uniform mat4 rotMatrix;
out vec4 vertcolor;
in vec2 inTexCoord;
out vec2 outTexCoord;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
void main(void)
{
	gl_Position = projectionMatrix*mdlMatrix*vec4(in_Position, 1.0);
	//vec3 norm = in_Normal.xyz;
	//vertcolor = vec4(norm.z,norm.z,norm.z, 1.0);
	outTexCoord = inTexCoord;
}
