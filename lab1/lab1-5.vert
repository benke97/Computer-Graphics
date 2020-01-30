#version 150

in  vec3 in_Position;
in  vec3 in_color;
uniform mat4 myMatrix;
uniform mat4 rotMatrix;
out vec3 vertcolor;
void main(void)
{
	gl_Position = rotMatrix*vec4(in_Position, 1.0);
	vertcolor = in_color;
}
