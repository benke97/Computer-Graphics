#version 150

out vec3 out_Color;
in vec3 vertcolor;

void main(void)
{
	out_Color = vertcolor;
}
