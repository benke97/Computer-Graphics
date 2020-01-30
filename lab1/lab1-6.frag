#version 150

out vec4 out_Color;
in vec4 vertcolor;

void main(void)
{
	out_Color = vertcolor;
}
