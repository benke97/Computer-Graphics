#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
out vec3 fragnormal;
out vec3 outPosition;

void main(void)
{
  outPosition = inPosition;
  fragnormal = inNormal;
	gl_Position = projMatrix*mdlMatrix*vec4(inPosition, 1.0);
}
